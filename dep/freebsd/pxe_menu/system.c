/* vim: ts=4
 */

#include "sysinstall.h"
#include <signal.h>
#include <termios.h>
#include <sys/reboot.h>
#include <machine/console.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysctl.h>


/* Where we stick our temporary expanded doc file */
#define	DOC_TMP_DIR	"/tmp/.doc"
#define	DOC_TMP_FILE	"/tmp/.doc/doc.tmp"

/*
 * Handle interrupt signals - this probably won't work in all cases
 * due to our having bogotified the internal state of dialog or curses,
 * but we'll give it a try.
 */
static int
intr_continue(dialogMenuItem *self)
{
    return DITEM_LEAVE_MENU;
}

static int
intr_reboot(dialogMenuItem *self)
{
    systemShutdown(-1);
    /* NOTREACHED */
    return 0;
}

static int
intr_restart(dialogMenuItem *self)
{
    int ret;

    free_variables();
    ret = execl(StartName, StartName, "-restart", (char *)NULL);
    msgDebug("execl failed (%s)\n", strerror(errno));
    /* NOTREACHED */
    return -1;
}

static dialogMenuItem intrmenu[] = {
    { "Abort",   "Abort the installation", NULL, intr_reboot },
    { "Restart", "Restart the installation program", NULL, intr_restart },
    { "Continue", "Continue the installation", NULL, intr_continue },
};


static void
handle_intr(int sig)
{
    WINDOW *save = savescr();

    use_helpline(NULL);
    use_helpfile(NULL);
    if (OnVTY) {
        ioctl(0, VT_ACTIVATE, 1);       /* Switch back */
        msgInfo(NULL);
    }
    (void)dialog_menu("Installation interrupt",
		     "Do you want to abort the installation?",
		     -1, -1, 3, -3, intrmenu, NULL, NULL, NULL);
    restorescr(save);
}

/* Expand a file into a convenient location, nuking it each time */
static char *
expand(char *fname)
{
    char *gunzip = RunningAsInit ? "/stand/gunzip" : "/usr/bin/gunzip";

    if (!directory_exists(DOC_TMP_DIR)) {
	Mkdir(DOC_TMP_DIR);
	if (chown(DOC_TMP_DIR, 0, 0) < 0)
	    return NULL;
	if (chmod(DOC_TMP_DIR, S_IRWXU) < 0)
	    return NULL;
    }
    else
	unlink(DOC_TMP_FILE);
    if (!file_readable(fname) || vsystem("%s < %s > %s", gunzip, fname, DOC_TMP_FILE))
	return NULL;
    return DOC_TMP_FILE;
}

/* Initialize system defaults */
void
systemInitialize(int argc, char **argv)
{
    size_t i;
    int boothowto;
    sigset_t signalset;

    signal(SIGINT, SIG_IGN);
    globalsInit();

    i = sizeof(boothowto);
    if (!sysctlbyname("debug.boothowto", &boothowto, &i, NULL, NULL) &&
        (i == sizeof(boothowto)) && (boothowto & RB_VERBOSE))
	variable_set2(VAR_DEBUG, "YES", 0);

    /* Are we running as init? */
    if (getpid() == 1) {
	int fd;

	RunningAsInit = 1;
	setsid();
	close(0);
	fd = open("/dev/ttyv0", O_RDWR);
	if (fd == -1) {
	    fd = open("/dev/console", O_RDWR);	/* fallback */
	    variable_set2(VAR_FIXIT_TTY, "serial", 0); /* give fixit a hint */
	} else
	    OnVTY = TRUE;
	/*
	 * To make _sure_ we're on a VTY and don't have /dev/console switched
	 * away to a serial port or something, attempt to set the cursor appearance.
	 */
	if (OnVTY) {
	    int fd2, type;

	    type = 0;	/* normal */
	    if ((fd2 = open("/dev/console", O_RDWR)) != -1) {
		if (ioctl(fd2, CONS_CURSORTYPE, &type) == -1) {
		    OnVTY = FALSE;
		    variable_set2(VAR_FIXIT_TTY, "serial", 0); /* Tell Fixit
								  the console
								  type */
		    close(fd); close(fd2);
		    open("/dev/console", O_RDWR);
		}
		else
		    close(fd2);
	    }
	}
	close(1); dup(0);
	close(2); dup(0);
	printf("%s running as init on %s\n", argv[0], OnVTY ? "vty0" : "serial console");
	ioctl(0, TIOCSCTTY, (char *)NULL);
	setlogin("root");
	setenv("PATH", "/stand:/bin:/sbin:/usr/sbin:/usr/bin:/mnt/bin:/mnt/sbin:/mnt/usr/sbin:/mnt/usr/bin:/usr/X11R6/bin", 1);
	setbuf(stdin, 0);
	setbuf(stderr, 0);
#ifdef __alpha__
	i = 0;
	sysctlbyname("machdep.unaligned_print", NULL, 0, &i, sizeof(i));
#endif
#if 0
	signal(SIGCHLD, reap_children);
#endif
    }
    else {
	char hname[256];

	/* Initalize various things for a multi-user environment */
	if (!gethostname(hname, sizeof hname))
	    variable_set2(VAR_HOSTNAME, hname, 0);
    }

    if (set_termcap() == -1) {
	printf("Can't find terminal entry\n");
	exit(-1);
    }

    /* XXX - libdialog has particularly bad return value checking */
    init_dialog();

    /* If we haven't crashed I guess dialog is running ! */
    DialogActive = TRUE;

    /* Make sure HOME is set for those utilities that need it */
    if (!getenv("HOME"))
	setenv("HOME", "/", 1);
    signal(SIGINT, handle_intr);
    /*
     * Make sure we can be interrupted even if we were re-executed
     * from an interrupt.
     */
    sigemptyset(&signalset);
    sigaddset(&signalset, SIGINT);
    sigprocmask(SIG_UNBLOCK, &signalset, NULL);

    (void)vsystem("rm -rf %s", DOC_TMP_DIR);
}

void
systemShutdown(int status)
{
    /* Shut down the dialog library */
    if (DialogActive) {
	end_dialog();
	DialogActive = FALSE;
    }

    /* Shut down curses */
    endwin();

    /* If we have a temporary doc dir lying around, nuke it */
    (void)vsystem("rm -rf %s", DOC_TMP_DIR);

    exit(status);
}

/* Run some general command */
int
systemExecute(char *command)
{
    int status;
    struct termios foo;
    WINDOW *w = savescr();

    dialog_clear();
    dialog_update();
    end_dialog();
    DialogActive = FALSE;
    if (tcgetattr(0, &foo) != -1) {
	foo.c_cc[VERASE] = '\010';
	tcsetattr(0, TCSANOW, &foo);
    }
    if (!Fake)
	status = system(command);
    else {
	status = 0;
	msgDebug("systemExecute:  Faked execution of `%s'\n", command);
    }
    DialogActive = TRUE;
    restorescr(w);
    return status;
}

/* suspend/resume libdialog/curses screen */
static    WINDOW *oldW;

void
systemSuspendDialog(void)
{

    oldW  = savescr();
    dialog_clear();
    dialog_update();
    end_dialog();
    DialogActive = FALSE;
}

void
systemResumeDialog(void)
{

    DialogActive = TRUE;
    restorescr(oldW);
}

/* Display a help file in a filebox */
int
systemDisplayHelp(char *file)
{
    char *fname = NULL;
    char buf[FILENAME_MAX];
    int ret = 0;
    WINDOW *w = savescr();
    
    fname = systemHelpFile(file, buf);
    if (!fname) {
	snprintf(buf, FILENAME_MAX, "The %s file is not provided on this particular floppy image.", file);
	use_helpfile(NULL);
	use_helpline(NULL);
	dialog_mesgbox("Sorry!", buf, -1, -1);
	ret = 1;
    }
    else {
	use_helpfile(NULL);
	use_helpline(NULL);
	dialog_textbox(file, fname, LINES, COLS);
    }
    restorescr(w);
    return ret;
}

char *
systemHelpFile(char *file, char *buf)
{
    if (!file)
	return NULL;
    if (file[0] == '/')
	return file;
    snprintf(buf, FILENAME_MAX, "/stand/help/%s.hlp.gz", file);
    if (file_readable(buf)) 
	return expand(buf);
    snprintf(buf, FILENAME_MAX, "/stand/help/%s.TXT.gz", file);
    if (file_readable(buf)) 
	return expand(buf);
    snprintf(buf, FILENAME_MAX, "/usr/src/release/sysinstall/help/%s.hlp", file);
    if (file_readable(buf))
	return buf;
    snprintf(buf, FILENAME_MAX, "/usr/src/release/sysinstall/help/%s.TXT", file);
    if (file_readable(buf))
	return buf;
    return NULL;
}

void
systemChangeTerminal(char *color, const u_char c_term[],
		     char *mono, const u_char m_term[])
{
    if (OnVTY) {
	int setupterm(char *color, int, int *);

	if (ColorDisplay) {
	    setenv("TERM", color, 1);
	    setenv("TERMCAP", c_term, 1);
	    reset_shell_mode();
	    setterm(color);
	    cbreak(); noecho();
	}
	else {
	    setenv("TERM", mono, 1);
	    setenv("TERMCAP", m_term, 1);
	    reset_shell_mode();
	    setterm(mono);
	    cbreak(); noecho();
	}
    }
    clear();
    refresh();
    dialog_clear();
}

int
vsystem(char *fmt, ...)
{
    va_list args;
    int pstat;
    pid_t pid;
    int omask;
    sig_t intsave, quitsave;
    char *cmd;
    int i;

    cmd = (char *)alloca(FILENAME_MAX);
    cmd[0] = '\0';
    va_start(args, fmt);
    vsnprintf(cmd, FILENAME_MAX, fmt, args);
    va_end(args);

    omask = sigblock(sigmask(SIGCHLD));
    if (Fake) {
	msgDebug("vsystem:  Faked execution of `%s'\n", cmd);
	return 0;
    }
    if (isDebug())
	msgDebug("Executing command `%s'\n", cmd);
    pid = fork();
    if (pid == -1) {
	(void)sigsetmask(omask);
	i = 127;
    }
    else if (!pid) {	/* Junior */
	(void)sigsetmask(omask);
	if (DebugFD != -1) {
	    dup2(DebugFD, 0);
	    dup2(DebugFD, 1);
	    dup2(DebugFD, 2);
	}
	else {
	    close(1); open("/dev/null", O_WRONLY);
	    dup2(1, 2);
	}
	if (!RunningAsInit)
	    execl("/bin/sh", "/bin/sh", "-c", cmd, (char *)NULL);
	else
	    execl("/stand/sh", "/stand/sh", "-c", cmd, (char *)NULL);
	exit(1);
    }
    else {
	intsave = signal(SIGINT, SIG_IGN);
	quitsave = signal(SIGQUIT, SIG_IGN);
	pid = waitpid(pid, &pstat, 0);
	(void)sigsetmask(omask);
	(void)signal(SIGINT, intsave);
	(void)signal(SIGQUIT, quitsave);
	i = (pid == -1) ? -1 : WEXITSTATUS(pstat);
	if (isDebug())
	    msgDebug("Command `%s' returns status of %d\n", cmd, i);
    }
    return i;
}

void
systemCreateHoloshell(void)
{
}
