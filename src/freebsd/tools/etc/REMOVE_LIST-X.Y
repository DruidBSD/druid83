# -*- tab-width:  4 -*- ;; Emacs
# vi: set tabstop=4     :: Vi/ViM
# vi: set filetype=sh   ::
############################################################ IDENT(1)
#
# $Title: host_rebuild list of files/directories to remove from FreeBSD-X.Y $
#
############################################################ COPYRIGHT
#
# (c)2012. Devin Teske. All Rights Reserved.
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
############################################################ INFORMATION
#
# List of files/directories that need to be deleted after upgrading to this
# particular FreeBSD release.
#
# The list should include items commonly found from older/newer releases that
# conflict with this release. Items that prevent smooth transition from one
# release to the next.
#
# Files/Directory pathnames should not be relative but can include patterns.
# Leading whitespace is trimmed and lines beginning with "#" are ignored.
# Spaces in filenames can be escaped with a single back-slash (\). Only one
# pathname per-line (anything after the first unescaped-space is ignored).
#
############################################################ FILES/DIRECTORIES

#
# Place files/directories to remove here
#

################################################################################
# END
################################################################################
#
# $Header: /cvsroot/druidbsd/druidbsd/druid83/src/freebsd/tools/etc/REMOVE_LIST-X.Y,v 1.1 2012/10/10 23:31:38 devinteske Exp $
#
# $Copyright: 2006-2012 Devin Teske. All rights reserved. $
#
# $Log: REMOVE_LIST-X.Y,v $
# Revision 1.1  2012/10/10 23:31:38  devinteske
# Commit initial public beta release of 8.3 series (beta 57; on-par with
# the 9.0 series released recently, also beta 57).
#
#
################################################################################
