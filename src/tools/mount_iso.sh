#!/bin/sh
# -*- tab-width:  4 -*- ;; Emacs
# vi: set tabstop=4     :: Vi/ViM
############################################################ COPYRIGHT
#
# (c)2011. Devin Teske. All Rights Reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
############################################################ GLOBALS

#
# Global exit status variables
#
SUCCESS=0
FAILURE=1

#
# Program name
#
progname="${0##*/}"

#
# OS Glue
#
UNAME_S=$( uname -s )

############################################################ FUNCTIONS

# have $anything
#
# A wrapper to the `type' built-in. Returns true if argument is a valid shell
# built-in, keyword, or externally-tracked binary, otherwise false.
#
have()
{
	type "$@" > /dev/null 2>&1
}

# fprintf $fd $fmt [ $opts ... ]
#
# Like printf, except allows you to print to a specific file-descriptor. Useful
# for printing to stderr (fd=2) or some other known file-descriptor.
#
fprintf()
{
	local fd=$1
	[ $# -gt 1 ] || return $FAILURE
	shift 1
	printf "$@" >&$fd
}

# eprintf $fmt [ $opts ... ]
#
# Print a message to stderr (fd=2).
#
eprintf()
{
	fprintf 2 "$@"
}

# die [ $fmt [ $opts ... ]]
#
# Optionally print a message to stderr before exiting with failure status.
#
die()
{
	local fmt="$1"
	[ $# -gt 0 ] && shift 1
	[  "$fmt"  ] && eprintf "$fmt\n" "$@"

	exit $FAILURE
}

# usage
#
# Prints a short syntax statement and exits.
#
usage()
{
	local argfmt1="\t%s\n" argfmt2="\t\t%s\n"
	local optfmt="\t%-6s%s\n"
	local exfmt="\t%s\n"

	eprintf "Usage: %s [OPTIONS] path_to_iso mount_point\n" "$progname"

	eprintf "ARGUMENTS:\n"
	eprintf "$argfmt1" "path_to_iso"
	eprintf "$argfmt2" \
	        "Location of ISO file (either relative or absolute) to be"
	eprintf "$argfmt2" \
	        "mounted. [Required]"
	eprintf "\n"
	eprintf "$argfmt1" "mount_point"
	eprintf "$argfmt2" \
	        "Directory to mount ISO file on (e.g., \`/mnt'). [Required]"
	eprintf "\n"

	eprintf "OPTIONS:\n"
	eprintf "$optfmt" "-h" \
	        "Print this message to stderr and exit."
	eprintf "\n"

	eprintf "EXAMPLE:\n"
	eprintf "$exfmt" \
	        "./$progname Druid-X.Y.iso /mnt"
	eprintf "\n"

	die
}

# eval2 $cmd ...
#
# Print a command to stdout before executing it.
#
eval2()
{
	echo "$*"
	eval "$@"
}

############################################################ MAIN SOURCE

[ $# -eq 2 ] || usage

ISOFILE="$1"
MOUNTDIR="$2"

#
# Perform sanity checks
#
[ -e "$MOUNTDIR" ] || die "%s:%s: No such file or directory." \
	"$progname" "$MOUNTDIR"
[ -d "$MOUNTDIR" ] || die "%s:%s: Not a directory." \
	"$progname" "$MOUNTDIR"

echo "Mounting ISO file: ${ISOFILE##*/}"
echo "...to destination: $MOUNTDIR"
echo ""
case "$UNAME_S" in
FreeBSD)
	if have mdconfig; then
		cmd="mdconfig -a -t vnode -u 99 -f"
		dev=/dev/md99
	elif have vnconfig; then
		cmd="vnconfig -c vn99"
		dev=/dev/vn99c
		if [ ! -e "$dev" ]; then
			( cd /dev && sh MAKEDEV vn99 )
			[ -e "$dev" ] || die "%s:$dev: %s" "$progname" \
				"Unable to create required device node"
		fi
	else
		die "%s: Unable to mount ISO file." "$progname"
	fi
	printf "Command1: "
	eval2 $cmd "$ISOFILE" || die
	printf "Command2: "
	eval2 mount -t cd9660 "$dev" "$MOUNTDIR" || die
	;;
Linux)
	printf "Command: "
	eval2 mount -o loop "$ISOFILE" "$MOUNTDIR" || die
	;;
*)
	die "%s: Unable to mount ISO file." "$progname"
esac
echo "done."

################################################################################
# END
################################################################################
#
# $Header: /cvsroot/druidbsd/druidbsd/druid83/src/tools/mount_iso.sh,v 1.1 2012/10/10 23:31:39 devinteske Exp $
#
# $Copyright: 2006-2012 Devin Teske. All rights reserved. $
#
# $Log: mount_iso.sh,v $
# Revision 1.1  2012/10/10 23:31:39  devinteske
# Commit initial public beta release of 8.3 series (beta 57; on-par with
# the 9.0 series released recently, also beta 57).
#
#
################################################################################
