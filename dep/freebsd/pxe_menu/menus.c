/* vim: ts=4
 */

#include "sysinstall.h"
#include <ctype.h>

/* Miscellaneous work routines for menus */
static int
checkTrue(dialogMenuItem *item)
{
		return TRUE;
}

/* The initial installation menu */
DMenu MenuInitial = {
    DMENU_CHECKLIST_TYPE,
    "Druid Main Menu",
	"Welcome to the DRUID FreeBSD installation menu. Please select one of\n"
	"the options below by using the arrow keys or typing the numerical value\n"
	"that appears to the left of the option name you're interested in. Invoke\n"
	"an option with [SPACE] or [ENTER].\n"
	"\n"
	"To exit (and return to the main menu of sysinstall(8)), use [TAB] to\n"
	"move to Cancel or invoke the last menu option.\n",
    NULL,
    NULL,
    { { NULL } },
};
char *configPaths[] = {
	"/etc/menu.conf",
	"/mnt/etc/menu.conf",
	"/mnt/menu.conf",
	"./menu.conf",
	"./etc/menu.conf",
	0,
};

/* XXX: The calling routine should free after use */
DMenu *
init_MenuInitial(void)
{
	DMenu	*menu = NULL;
	int		msize, count = 0, i;
	char	*err = NULL;

	msize = sizeof(DMenu) + (sizeof(dialogMenuItem) * 5);
	if ((menu = malloc(msize)) == NULL) {
		err = "Failed to allocate memory for menu";
		goto err;
	}

	bcopy(&MenuInitial, menu, sizeof(DMenu));

	/* Add OK button */
	bzero(&menu->items[count], sizeof(menu->items[0]));
	menu->items[count].prompt = strdup("  OK  ");
	count++;

	/* Add Cancel button */
	bzero(&menu->items[count], sizeof(menu->items[0]));
	menu->items[count].prompt = strdup("Cancel");
	menu->items[count].fire = dmenuExit;
	count++;

	/* Add option to toggle diskInteractive */
	bzero(&menu->items[count], sizeof(menu->items[0]));
	if ((menu->items[count].prompt =
	    (char *)malloc(sizeof(int)*3+4)) == NULL) {
		err = "Failed to allocate memory for menu item prompt";
		goto err;
	}
	menu->items[count].data = strdup("diskInteractive=YES");
	snprintf(menu->items[count].prompt, sizeof(int)*3-1, "%i", count-1);
	menu->items[count].title = strdup("Interactive Disk Partitioning");
	menu->items[count].checked = dmenuVarCheck;
	menu->items[count].fire = dmenuToggleVariable;
	count++;

	/* Try to open our configuration file */
	for (i = 0; configPaths[i]; i++)
	{
		FILE *file = fopen(configPaths[i], "r");
		if (!file) continue;

		while (1)
		{
			char *line, *curpos, *pos, *data, c;
			size_t len, n;
			dialogMenuItem *item;

			/* get the next line */
			line = fgetln(file, &len);
			if (feof(file)) break;
			if (!len) continue; /* skip blank lines */

			/* terminate the line */
			c = line[len];
			line[len] = 0;

			/* skip leading whitespace */
			curpos = line;
			while (*curpos && isspace(*curpos)) curpos++;
			if (!*curpos) goto next_line;
			if (*curpos == '#') goto next_line;

			/* find the end of the command */
			pos = curpos;
			while (*pos && !isspace(*pos) && *pos != '#') pos++;

			/* add an item */
			msize += sizeof(dialogMenuItem);
			if ((menu = realloc(menu, msize)) == NULL) {
				err = "Failed to allocate memory for menu item";
				fclose(file); goto err;
			}
			bzero(&menu->items[count], sizeof(menu->items[0]));
			item = &menu->items[count];

			/* store the command as data */
			if ((item->data = (char *)malloc(pos - curpos + 1)) == NULL) {
				err = "Failed to allocate memory for menu item data";
				fclose(file); goto err;
			}
			data = item->data;
			n = snprintf(item->data, pos - curpos + 1, "%s", curpos);
			data[n-1] = 0;

			/* add the item prompt */
			if ((item->prompt = (char *)malloc(sizeof(int)*3)) == NULL) {
				err = "Failed to allocate memory for menu item prompt";
				fclose(file); goto err;
			}
			snprintf(item->prompt,sizeof(int)*3-1,"%i",count-1);

			/* add the item title */
			while (*pos && isspace(*pos) && *pos != '#') pos++;
			if (*pos && *pos != '#') {
				curpos = pos;
				while (*pos && *pos != '#') pos++;
				if (*pos == '#') {
					pos--;
					while (isspace(*pos)) pos--;
					pos++;
				}
				if ((item->title = (char *)malloc(pos - curpos + 1)) == NULL) {
					err = "Failed to allocate memory for menu item title";
					fclose(file); goto err;
				}
				n = snprintf(item->title, pos - curpos + 1, "%s", curpos);
				item->title[n-1] = 0;
			}
			else {
				item->title = strdup(item->data);
			}

			item->fire = dmenuInstallScript;
			item->lbra = ' ';
			item->mark = ' ';
			item->rbra = ' ';

			count++;
next_line:
			/* unterminate the line */
			line[len] = c;
		}

		fclose(file);
	}

	/* Add option to drop back to sysinstall */
	bzero(&menu->items[count], sizeof(menu->items[0]));
	if ((menu->items[count].prompt =
	    (char *)malloc(sizeof(int)*3)) == NULL) {
		err = "Failed to allocate memory for menu item prompt";
		goto err;
	}
	snprintf(menu->items[count].prompt, sizeof(int)*3-1, "%i", count-1);
	menu->items[count].title = 
		strdup("Advanced Menu (Perform Manual/Expert Install)");
	menu->items[count].checked = checkTrue;
	menu->items[count].fire = dmenuExit;
	menu->items[count].lbra = '<';
	menu->items[count].mark = '<';
	menu->items[count].rbra = '<';
	count++;

	/* Terminate item array */
    	menu->items[count].prompt = NULL;
    	menu->items[count].title = NULL;

	return menu;

err:
	for (i = 0; i < count; i++)
		free(menu->items[i].prompt);
	free(menu);
	if (err != NULL)
		msgConfirm(err);
	return NULL;
}
