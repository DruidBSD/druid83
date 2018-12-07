/* vim: ts=8
 */

#include "sysinstall.h"

/*
 * Various global variables and an initialization hook to set them to
 * whatever values we feel are appropriate.
 */

int		DebugFD;	/* Where diagnostic output goes */
Boolean		Fake;		/* Only pretend to be useful */
Boolean		RunningAsInit;	/* Are we running as init? */
Boolean		DialogActive;	/* Is libdialog initialized? */
Boolean		ColorDisplay;	/* Are we on a color display? */
Boolean		OnVTY;		/* Are we on a VTY? */
Boolean		Restarting;	/* Are we restarting sysinstall? */
Variable	*VarHead;	/* The head of the variable chain */
int		BootMgr;	/* Which boot manager we're using */
int		StatusLine;	/* Where to stick our status messages */

/*
 * Yes, I know some of these are already automatically initialized as
 * globals.  I simply find it clearer to set everything explicitly.
 */
void
globalsInit(void)
{
	DebugFD = -1;
	ColorDisplay = FALSE;
	Fake = FALSE;
	Restarting = FALSE;
	OnVTY = FALSE;
	DialogActive = FALSE;
	VarHead = NULL;
	RunningAsInit = FALSE;
}
