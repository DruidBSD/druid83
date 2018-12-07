/* vim: ts=4
 */

#include "sysinstall.h"

int
dmenuInstallScript(dialogMenuItem *self)
{
	int status;
	char *cmd = (char *)self->data;

	dialog_clear();

	if (cmd && *cmd == '!')
		cmd++;
	else if (msgYesNo(
		"Last Chance!  Are you SURE you want to continue the installation?\n\n"
		"If you're running this on a disk with data you wish to save\n"
		"then WE STRONGLY ENCOURAGE YOU TO MAKE PROPER BACKUPS before\n"
		"proceeding!\n\n"
		"We can take no responsibility for lost disk contents!") != 0)
		return DITEM_LEAVE_MENU;

	status = systemExecute(cmd);

	if (!status)
	{
		/* command was successful (return control to init) */
		dialog_clear();
		systemShutdown(0);
	}
	else if ((status >> 8) != 255)
		msgConfirm("Error executing script '%s'!", cmd);

	return DITEM_LEAVE_MENU;
}
