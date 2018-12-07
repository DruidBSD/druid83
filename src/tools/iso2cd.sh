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

	local device="cd0"
	case "$UNAME_S" in
	Linux) device="cdwriter"
	esac

	eprintf "Usage: %s [OPTIONS] path_to_iso device_name\n" "$progname"

	eprintf "ARGUMENTS:\n"
	eprintf "$argfmt1" "path_to_iso"
	eprintf "$argfmt2" \
	        "Location of ISO file (either relative or absolute) to be"
	eprintf "$argfmt2" \
	        "written to CD-ROM. [Required]"
	eprintf "\n"
	eprintf "$argfmt1" "device_name"
	eprintf "$argfmt2" \
	        "Device name (or path) of CD-R/RW burner device (e.g.,"
	eprintf "$argfmt2" \
	        "\`$device' or \`/dev/$device') to write to."
	eprintf "\n"

	eprintf "OPTIONS:\n"
	eprintf "$optfmt" "-h" \
	        "Print this message to stderr and exit."
	eprintf "\n"

	eprintf "EXAMPLE:\n"
	eprintf "$exfmt" \
	        "./$progname Druid-X.Y.iso $device"
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

[ $# -ge 1 ] || usage

ISOFILE="$1"
CDDEV="$2"

case "$CDDEV" in
*/*) : nothing to change;;
*)
	[ "$CDDEV" -a -e "/dev/$CDDEV" ] && CDDEV="/dev/$CDDEV:@"
esac

echo "Copying data from: ${ISOFILE##*/}"
echo "...to destination: $CDDEV"
echo ""
echo -n "Command: "
eval2 cdrecord -v ${CDDEV:+dev="$CDDEV"} "$ISOFILE" driveropts=burnfree \&
pid=$!
echo "Process-id: $pid"

trap "kill -9 $pid" INT
wait $pid

################################################################################
# END
################################################################################
#
# $Header: /cvsroot/druidbsd/druidbsd/druid83/src/tools/iso2cd.sh,v 1.1 2012/10/10 23:31:39 devinteske Exp $
#
# $Copyright: 2006-2012 Devin Teske. All rights reserved. $
#
# $Log: iso2cd.sh,v $
# Revision 1.1  2012/10/10 23:31:39  devinteske
# Commit initial public beta release of 8.3 series (beta 57; on-par with
# the 9.0 series released recently, also beta 57).
#
#
################################################################################
