#!/bin/sh
# -*- tab-width:  4 -*- ;; Emacs
# vi: set tabstop=4     :: Vi/ViM

############################################################ GLOBALS

#
# CHROOTDIR is defined by the release(7) process
#

############################################################ MAIN SOURCE

echo $0 "$@"
set -o errexit -o nounset -o xtrace

#
# Due to the changes applied via `world_patches/include::paths.h.patch', we
# need to give ourselves a working shell, else system-calls such as popen(3)
# will fail (e.g., crunchgen(1)).
#
# In the above patch, we ``lock-down'' _PATH_DEFPATH, _PATH_STDPATH,
# _PATH_SYSPATH, and _PATH_BSHELL (among others) to be rooted to `/stand' which
# can effect our release-process which calls some of the executables compiled
# with these changes. 
#
mkdir -pv $CHROOTDIR/stand
cp -fv $CHROOTDIR/bin/sh $CHROOTDIR/stand

################################################################################
#
################################################################################
#
# $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/patches/local_script.sh,v 1.1 2012/10/10 23:30:00 devinteske Exp $
#
# $Copyright: 2006-2012 Devin Teske. All rights reserved. $
#
# $Log: local_script.sh,v $
# Revision 1.1  2012/10/10 23:30:00  devinteske
# Commit initial public beta release of 8.3 series (beta 57; on-par with
# the 9.0 series released recently, also beta 57).
#
#
################################################################################
