/* vim: ts=4
 */

#include "sysinstall.h"
#include <stdarg.h>
#include <sys/ioctl.h>
#include <machine/console.h>

Boolean
isDebug(void)
{
    char *cp;

    return (cp = variable_get(VAR_DEBUG)) && strcmp(cp, "no");
}

/* Whack up an informational message on the status line, in stand-out */
void
msgYap(char *fmt, ...)
{
    va_list args;
    char *errstr;
    int attrs;

    errstr = (char *)alloca(FILENAME_MAX);
    va_start(args, fmt);
    vsnprintf(errstr, FILENAME_MAX, fmt, args);
    va_end(args);
    attrs = getattrs(stdscr);
    attrset(A_REVERSE);
    mvaddstr(StatusLine, 0, errstr);
    attrset(attrs);
    refresh();
}

/* Whack up an informational message on the status line */
void
msgInfo(char *fmt, ...)
{
    va_list args;
    char *errstr;
    int i, attrs;
    char line[81];

    attrs = getattrs(stdscr);
    /* NULL is a special convention meaning "erase the old stuff" */
    if (!fmt) {
	move(StatusLine, 0);
	clrtoeol();
	return;
    }
    errstr = (char *)alloca(FILENAME_MAX);
    va_start(args, fmt);
    vsnprintf(errstr, FILENAME_MAX, fmt, args);
    va_end(args);
    memset(line, ' ', 80);
    for (i = 0; i < 80; i++) {
	if (errstr[i])
	    line[i] = errstr[i];
	else
	    break;
    }
    line[80] = '\0';
    attrset(ATTR_TITLE);
    mvaddstr(StatusLine, 0, line);
    attrset(attrs);
    move(StatusLine, 79);
    refresh();
}

/* Whack up a warning on the status line */
void
msgWarn(char *fmt, ...)
{
    va_list args;
    char *errstr;
    int attrs;

    errstr = (char *)alloca(FILENAME_MAX);
    strcpy(errstr, "Warning: ");
    va_start(args, fmt);
    vsnprintf((char *)(errstr + strlen(errstr)), FILENAME_MAX, fmt, args);
    va_end(args);
    attrs = getattrs(stdscr);
    beep();
    attrset(ATTR_TITLE);
    mvaddstr(StatusLine, 0, errstr);
    attrset(attrs);
    refresh();
    if (OnVTY && isDebug())
	msgDebug("Warning message `%s'\n", errstr);
}

/* Whack up an error on the status line */
void
msgError(char *fmt, ...)
{
    va_list args;
    char *errstr;
    int attrs;

    errstr = (char *)alloca(FILENAME_MAX);
    strcpy(errstr, "Error: ");
    va_start(args, fmt);
    vsnprintf((char *)(errstr + strlen(errstr)), FILENAME_MAX, fmt, args);
    va_end(args);
    beep();
    attrs = getattrs(stdscr);
    attrset(ATTR_TITLE);
    mvaddstr(StatusLine, 0, errstr);
    attrset(attrs);
    refresh();
    if (OnVTY && isDebug())
	msgDebug("Error message `%s'\n", errstr);
}

/* Whack up a fatal error on the status line */
void
msgFatal(char *fmt, ...)
{
    va_list args;
    char *errstr;
    int attrs;

    errstr = (char *)alloca(FILENAME_MAX);
    strcpy(errstr, "Fatal Error: ");
    va_start(args, fmt);
    vsnprintf((char *)(errstr + strlen(errstr)), FILENAME_MAX, fmt, args);
    va_end(args);
    beep();
    attrs = getattrs(stdscr);
    attrset(ATTR_TITLE);
    mvaddstr(StatusLine, 0, errstr);
    addstr(" - ");
    addstr("PRESS ANY KEY TO ");
    if (getpid() == 1)
	addstr("REBOOT");
    else
	addstr("QUIT");
    attrset(attrs);
    refresh();
    if (OnVTY)
	msgDebug("Fatal error `%s'!\n", errstr);
    getch();
    systemShutdown(1);
}

/* Put up a message in a popup confirmation box */
void
msgConfirm(char *fmt, ...)
{
    va_list args;
    char *errstr;
    WINDOW *w = savescr();

    errstr = (char *)alloca(FILENAME_MAX);
    va_start(args, fmt);
    vsnprintf(errstr, FILENAME_MAX, fmt, args);
    va_end(args);
    use_helpline(NULL);
    use_helpfile(NULL);
    if (OnVTY) {
	ioctl(0, VT_ACTIVATE, 1);
	msgInfo(NULL);
    }
    dialog_notify(errstr);
    restorescr(w);
}

/* Put up a message in a popup information box */
void
msgNotify(char *fmt, ...)
{
    va_list args;
    char *errstr;

    errstr = (char *)alloca(FILENAME_MAX);
    va_start(args, fmt);
    vsnprintf(errstr, FILENAME_MAX, fmt, args);
    va_end(args);
    use_helpline(NULL);
    use_helpfile(NULL);
    if (isDebug())
	msgDebug("Notify: %s\n", errstr);
    dialog_msgbox(NULL, errstr, -1, -1, 0);
}

/* Put up a message in a popup yes/no box and return 0 for YES, 1 for NO */
int
msgYesNo(char *fmt, ...)
{
    va_list args;
    char *errstr;
    int ret;
    WINDOW *w = savescr();
    
    errstr = (char *)alloca(FILENAME_MAX);
    va_start(args, fmt);
    vsnprintf(errstr, FILENAME_MAX, fmt, args);
    va_end(args);
    use_helpline(NULL);
    use_helpfile(NULL);
    if (OnVTY) {
	ioctl(0, VT_ACTIVATE, 1);	/* Switch back */
	msgInfo(NULL);
    }
#if 0
    if (variable_get(VAR_NONINTERACTIVE))
	return 0;	/* If non-interactive, return YES all the time */
#endif
    ret = dialog_yesno("User Confirmation Requested", errstr, -1, -1);
    restorescr(w);
    return ret;
}

/* Put up a message in a popup no/yes box and return 0 for YES, 1 for NO */
int
msgNoYes(char *fmt, ...)
{
    va_list args;
    char *errstr;
    int ret;
    WINDOW *w = savescr();
    
    errstr = (char *)alloca(FILENAME_MAX);
    va_start(args, fmt);
    vsnprintf(errstr, FILENAME_MAX, fmt, args);
    va_end(args);
    use_helpline(NULL);
    use_helpfile(NULL);
    if (OnVTY) {
	ioctl(0, VT_ACTIVATE, 1);	/* Switch back */
	msgInfo(NULL);
    }
    if (variable_get(VAR_NONINTERACTIVE))
	return 1;	/* If non-interactive, return NO all the time */
    ret = dialog_noyes("User Confirmation Requested", errstr, -1, -1);
    restorescr(w);
    return ret;
}

/* Put up a message in an input box and return the value */
char *
msgGetInput(char *buf, char *fmt, ...)
{
    va_list args;
    char *errstr;
    static char input_buffer[256];
    int rval;
    WINDOW *w = savescr();

    errstr = (char *)alloca(FILENAME_MAX);
    va_start(args, fmt);
    vsnprintf(errstr, FILENAME_MAX, fmt, args);
    va_end(args);
    use_helpline(NULL);
    use_helpfile(NULL);
    if (buf)
	SAFE_STRCPY(input_buffer, buf);
    else
	input_buffer[0] = '\0';
    if (OnVTY) {
	ioctl(0, VT_ACTIVATE, 1);	/* Switch back */
	msgInfo(NULL);
    }
    rval = dialog_inputbox("Value Required", errstr, -1, -1, input_buffer);
    restorescr(w);
    if (!rval)
	return input_buffer;
    else
	return NULL;
}

/* Write something to the debugging port */
void
msgDebug(char *fmt, ...)
{
    va_list args;
    char *dbg;

    if (DebugFD == -1)
	return;
    dbg = (char *)alloca(FILENAME_MAX);
    strcpy(dbg, "DEBUG: ");
    va_start(args, fmt);
    vsnprintf((char *)(dbg + strlen(dbg)), FILENAME_MAX, fmt, args);
    va_end(args);
    write(DebugFD, dbg, strlen(dbg));
}

/* Tell the user there's some output to go look at */
void
msgWeHaveOutput(char *fmt, ...)
{
    va_list args;
    char *errstr;
    WINDOW *w = savescr();
    
    errstr = (char *)alloca(FILENAME_MAX);
    va_start(args, fmt);
    vsnprintf(errstr, FILENAME_MAX, fmt, args);
    va_end(args);
    use_helpline(NULL);
    use_helpfile(NULL);
    msgDebug("Notify: %s\n", errstr);
    dialog_clear_norefresh();
    sleep(2);
    dialog_msgbox(NULL, errstr, -1, -1, 0);
    restorescr(w);
}

/* Simple versions of msgConfirm() and msgNotify() for calling from scripts */
int
msgSimpleConfirm(char *str)
{
    msgConfirm("%s", str);
    return DITEM_SUCCESS;
}

int
msgSimpleNotify(char *str)
{
    msgNotify("%s", str);
    return DITEM_SUCCESS;
}
