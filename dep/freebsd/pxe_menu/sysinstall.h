#ifndef _SYSINSTALL_H_INCLUDE
#define _SYSINSTALL_H_INCLUDE

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dialog.h>
#include "ui_objects.h"
#include "dir.h"
#include "colors.h"
#include "libdisk.h"

/*** Defines ***/

/* Number of seconds to wait for data to come off even the slowest media */
#define MEDIA_TIMEOUT		300

#define SYSTEM_STATE			"_systemState"

/* Ones that can be tweaked from config files */
#define VAR_BROWSER_BINARY		"browserBinary"
#define VAR_BROWSER_PACKAGE		"browserPackage"
#define VAR_CPIO_VERBOSITY		"cpioVerbose"
#define VAR_DEBUG			"debug"
#define VAR_EDITOR			"editor"
#define VAR_FIXIT_TTY			"fixitTty"
#define VAR_FTP_STATE			"ftpState"
#define VAR_FTP_USER			"ftpUser"
#define VAR_HOSTNAME			"hostname"
#define VAR_INSTALL_CFG			"installConfig"
#define VAR_INSTALL_ROOT		"installRoot"
#define VAR_KEYMAP			"keymap"
#define VAR_KGET			"kget"
#define VAR_MEDIA_TIMEOUT		"MEDIA_TIMEOUT"
#define VAR_NEWFS_ARGS			"newfsArgs"
#define VAR_NFS_SECURE			"nfs_reserved_port_only"
#define VAR_NONINTERACTIVE		"nonInteractive"
#define VAR_PKG_TMPDIR			"PKG_TMPDIR"
#define VAR_RELNAME			"releaseName"
#define VAR_TAPE_BLOCKSIZE		"tapeBlocksize"

#define DEFAULT_TAPE_BLOCKSIZE	"20"

/* Which selection attributes to use */
#define ATTR_TITLE	button_active_attr

/* Handy strncpy() macro */
#define SAFE_STRCPY(to, from)	sstrncpy((to), (from), sizeof (to) - 1)

/*** Types ***/
typedef int Boolean;
typedef struct chunk Chunk;

/* Bitfields for menu options */
#define DMENU_NORMAL_TYPE	0x1     /* Normal dialog menu           */
#define DMENU_RADIO_TYPE	0x2     /* Radio dialog menu            */
#define DMENU_CHECKLIST_TYPE	0x4     /* Multiple choice menu         */
#define DMENU_SELECTION_RETURNS 0x8     /* Immediate return on item selection */

typedef struct _dmenu {
    int type;				/* What sort of menu we are	*/
    char *title;			/* Our title			*/
    char *prompt;			/* Our prompt			*/
    char *helpline;			/* Line of help at bottom	*/
    char *helpfile;			/* Help file for "F1"		*/
#if (__STDC_VERSION__ >= 199901L) || (__GNUC__ >= 3) 
    dialogMenuItem items[];		/* Array of menu items		*/
#elif __GNUC__
    dialogMenuItem items[0];		/* Array of menu items		*/
#else
#error "Create hack for C89 and K&R compilers."
#endif
} DMenu;

/* An rc.conf variable */
typedef struct _variable {
    struct _variable *next;
    char *name;
    char *value;
    int dirty;
} Variable;

#define NO_ECHO_OBJ(type)	((type) | (DITEM_NO_ECHO << 16))
#define TYPE_OF_OBJ(type)	((type) & 0xff)
#define ATTR_OF_OBJ(type)	((type) >> 16)

/* A screen layout structure */
typedef struct _layout {
    int         y;              /* x & Y co-ordinates */
    int         x;
    int         len;            /* The size of the dialog on the screen */
    int         maxlen;         /* How much the user can type in ... */
    char        *prompt;        /* The string for the prompt */
    char        *help;          /* The display for the help line */
    void        *var;           /* The var to set when this changes */
    int         type;           /* The type of the dialog to create */
    void        *obj;           /* The obj pointer returned by libdialog */
} Layout;

/* An option */
typedef struct _opt {
    char *name;
    char *desc;
    enum { OPT_IS_STRING, OPT_IS_INT, OPT_IS_FUNC, OPT_IS_VAR } type;
    void *data;
    void *aux;
    char *(*check)();
} Option;


/*** Externs ***/
extern int		DebugFD;		/* Where diagnostic output goes			*/
extern Boolean		Fake;			/* Don't actually modify anything - testing	*/
extern Boolean		Restarting;		/* Are we restarting sysinstall?		*/
extern Boolean		RunningAsInit;		/* Are we running stand-alone?			*/
extern Boolean		DialogActive;		/* Is the dialog() stuff up?			*/
extern Boolean		ColorDisplay;		/* Are we on a color display?			*/
extern Boolean		OnVTY;			/* On a syscons VTY?				*/
extern Variable		*VarHead;		/* The head of the variable chain		*/
extern int		BootMgr;		/* Which boot manager to use 			*/
extern int		StatusLine;		/* Where to print our status messages		*/
extern DMenu		MenuInitial;		/* Initial installation menu	*/
extern DMenu		MenuOptions;		/* Options menu	*/
extern const char *	StartName;		/* Which name we were started as */

/* Stuff from libdialog which isn't properly declared outside */
extern void display_helpfile(void);
extern void display_helpline(WINDOW *w, int y, int width);

/*** Prototypes ***/

/* menus.c */
extern DMenu *	init_MenuInitial(void);

/* install.c */
extern int	dmenuInstallScript(dialogMenuItem *tmp);

/* dmenu.c */
extern int	dmenuDisplayFile(dialogMenuItem *tmp);
extern int	dmenuSubmenu(dialogMenuItem *tmp);
extern int	dmenuSystemCommand(dialogMenuItem *tmp);
extern int	dmenuSystemCommandBox(dialogMenuItem *tmp);
extern int	dmenuExit(dialogMenuItem *tmp);
extern int	dmenuISetVariable(dialogMenuItem *tmp);
extern int	dmenuSetVariable(dialogMenuItem *tmp);
extern int	dmenuSetKmapVariable(dialogMenuItem *tmp);
extern int	dmenuSetVariables(dialogMenuItem *tmp);
extern int	dmenuToggleVariable(dialogMenuItem *tmp);
extern int	dmenuSetFlag(dialogMenuItem *tmp);
extern int	dmenuSetValue(dialogMenuItem *tmp);
extern Boolean	dmenuOpen(DMenu *menu, int *choice, int *scroll, int *curr, int *max, Boolean buttons);
extern Boolean	dmenuOpenSimple(DMenu *menu, Boolean buttons);
extern int	dmenuVarCheck(dialogMenuItem *item);
extern int	dmenuVarsCheck(dialogMenuItem *item);
extern int	dmenuFlagCheck(dialogMenuItem *item);
extern int	dmenuRadioCheck(dialogMenuItem *item);

/* globals.c */
extern void	globalsInit(void);

/* install.c */
extern Boolean	checkLabels(Boolean whinge, Chunk **rdev, Chunk **sdev, Chunk **udev, Chunk **vdev, Chunk **vtdev, Chunk **hdev);
extern int	installFixupXFree(dialogMenuItem *self);
extern int	installX11package(dialogMenuItem *self);
extern Boolean	copySelf(void);

/* keymap.c */
extern int	loadKeymap(const char *lang);

/* makedevs.c (auto-generated) */
extern const char	termcap_ansi[];
extern const char	termcap_vt100[];
extern const char	termcap_cons25w[];
extern const char	termcap_cons25[];
extern const char	termcap_cons25_m[];
extern const char	termcap_cons25r[];
extern const char	termcap_cons25r_m[];
extern const char	termcap_cons25l1[];
extern const char	termcap_cons25l1_m[];
extern const char	termcap_xterm[];
extern const u_char	font_iso_8x16[];
extern const u_char	font_cp850_8x16[];
extern const u_char	font_cp866_8x16[];
extern const u_char	koi8_r2cp866[];
extern u_char		default_scrnmap[];

/* misc.c */
extern Boolean	file_readable(char *fname);
extern Boolean	file_executable(char *fname);
extern Boolean	directory_exists(const char *dirname);
extern char	*root_bias(char *path);
extern char	*itoa(int value);
extern char	*string_concat(char *p1, char *p2);
extern char	*string_concat3(char *p1, char *p2, char *p3);
extern char	*string_prune(char *str);
extern char	*string_skipwhite(char *str);
extern char	*string_copy(char *s1, char *s2);
extern char	*pathBaseName(const char *path);
extern void	safe_free(void *ptr);
extern void	*safe_malloc(size_t size);
extern void	*safe_realloc(void *orig, size_t size);
extern dialogMenuItem *item_add(dialogMenuItem *list, char *prompt, char *title, 
				int (*checked)(dialogMenuItem *self),
				int (*fire)(dialogMenuItem *self),
				void (*selected)(dialogMenuItem *self, int is_selected),
				void *data, int aux, int *curr, int *max);
extern void	items_free(dialogMenuItem *list, int *curr, int *max);
extern int	Mkdir(char *);
extern int	Mount(char *, void *data);
extern WINDOW	*openLayoutDialog(char *helpfile, char *title, int x, int y, int width, int height);
extern ComposeObj *initLayoutDialog(WINDOW *win, Layout *layout, int x, int y, int *max);
extern int	layoutDialogLoop(WINDOW *win, Layout *layout, ComposeObj **obj,
				 int *n, int max, int *cbutton, int *cancel);

extern WINDOW	*savescr(void);
extern void	restorescr(WINDOW *w);
extern char	*sstrncpy(char *dst, const char *src, int size);

/* msg.c */
extern Boolean	isDebug(void);
extern void	msgInfo(char *fmt, ...) __printf0like(1, 2);
extern void	msgYap(char *fmt, ...) __printflike(1, 2);
extern void	msgWarn(char *fmt, ...) __printflike(1, 2);
extern void	msgDebug(char *fmt, ...) __printflike(1, 2);
extern void	msgError(char *fmt, ...) __printflike(1, 2);
extern void	msgFatal(char *fmt, ...) __printflike(1, 2);
extern void	msgConfirm(char *fmt, ...) __printflike(1, 2);
extern void	msgNotify(char *fmt, ...) __printflike(1, 2);
extern void	msgWeHaveOutput(char *fmt, ...) __printflike(1, 2);
extern int	msgYesNo(char *fmt, ...) __printflike(1, 2);
extern int	msgNoYes(char *fmt, ...) __printflike(1, 2);
extern char	*msgGetInput(char *buf, char *fmt, ...) __printflike(2, 3);
extern int	msgSimpleConfirm(char *);
extern int	msgSimpleNotify(char *);

/* system.c */
extern void	systemInitialize(int argc, char **argv);
extern void	systemShutdown(int status);
extern int	execExecute(char *cmd, char *name);
extern int	systemExecute(char *cmd);
extern void	systemSuspendDialog(void);
extern void	systemResumeDialog(void);
extern int	systemDisplayHelp(char *file);
extern char	*systemHelpFile(char *file, char *buf);
extern void	systemChangeFont(const u_char font[]);
extern void	systemChangeLang(char *lang);
extern void	systemChangeTerminal(char *color, const u_char c_termcap[], char *mono, const u_char m_termcap[]);
extern void	systemChangeScreenmap(const u_char newmap[]);
extern void	systemCreateHoloshell(void);
extern int	vsystem(char *fmt, ...) __printflike(1, 2);

/* termcap.c */
extern int	set_termcap(void);

/* variable.c */
extern void	variable_set(char *var, int dirty);
extern void	variable_set2(char *name, char *value, int dirty);
extern char 	*variable_get(char *var);
extern int 	variable_cmp(char *var, char *value);
extern void	variable_unset(char *var);
extern char	*variable_get_value(char *var, char *prompt, int dirty);
extern int 	variable_check(char *data);
extern int	dump_variables(dialogMenuItem *self);
extern void	free_variables(void);
extern void     pvariable_set(char *var);
extern char     *pvariable_get(char *var);

#endif
/* _SYSINSTALL_H_INCLUDE */
