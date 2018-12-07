/* vim: ts=4
 */

#include "sysinstall.h"
#include <sys/signal.h>
#include <sys/fcntl.h>

const char *StartName;		/* Initial contents of argv[0] */

int
main(int argc, char *argv[])
{
	int choice, scroll, curr, max;
	DMenu *menu;
    
	/* Record name to be able to restart */
	StartName = argv[0];

#ifdef PC98
	{
		/* XXX */
		char *p = getenv("TERM");
		if (p && strcmp(p, "cons25") == 0)
			putenv("TERM=cons25w");
	}
#endif

	/* Move stderr aside */
	if (DebugFD)
		dup2(DebugFD, 2);

	/* Set up whatever things need setting up */
	systemInitialize(argc, argv);
	dialog_clear();

	choice = scroll = curr = max = 0;
#if 0
	dmenuOpen(MenuInitial, &choice, &scroll, &curr, &max, TRUE);
#else
	menu = init_MenuInitial();
	if (menu)
		dmenuOpen(menu, &choice, &scroll, &curr, &max, TRUE);
#endif

	/* Say goodnight, Gracie */
	dialog_clear();
	systemShutdown(0);

	return 0; /* We should never get here */
}
