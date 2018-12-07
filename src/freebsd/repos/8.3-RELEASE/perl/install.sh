#!/bin/sh
#
# $Header: /cvsroot/druidbsd/druidbsd/druid83/src/freebsd/repos/8.3-RELEASE/perl/install.sh,v 1.1 2012/10/10 23:30:48 devinteske Exp $
#

if [ "`id -u`" != "0" ]; then
	echo "Sorry, this must be done as root."
	exit 1
fi
cat perl.?? | tar --unlink -xpzf - -C ${DESTDIR:-/}
exit 0
