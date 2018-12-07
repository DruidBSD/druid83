############################## INTRODUCTION

Hello and welcome to the Druid!

The Druid is many things:

   - A cross-platform build system
   - A bootable disc image with tools and utilities
   - A way to install FreeBSD
   - A way to distribute firmware updates to the field
   - A solution to a multitude of problems/frustrations
   - A solution to efficiency by automation and customization

Most notably, it's a framework intended to be used by many developers. A
framework for developing bootable disc images with an easy-to-use intuitive
interface.

As you will see in the below instructions, you will `Configure' the Druid once,
but be able to build many things from there. Read carefully below to learn
which options build what, as each sub-project may build something entirely
different with the Druid framework.


############################## Configuration

First, we need to configure Druid to recognize your build environment.

	$ ./configure

Then,...

############################## Build

NOTE: On FreeBSD, substitute "gmake" wherever "make" is used

To build the FreeBSD Druid ISO:

	$ make freebsd

	Creates: FreeBSD-A.B_Druid-X.Y.iso

	NOTE: Also contains `Diagnostic/Repair Tools'.

To build the Diagnostic/Repair Tools ISO:

	$ make tools

	Creates: Druid_Tools-X.Y.iso

To build the All-In-One Druid ISO (containing all of the above):

	$ make

	Creates: Druid-X.Y.iso

To build the Kitchen-Sink Developer's Edition (which may contain preview
software such as FreeBSD-10.x):

	$ make dev

	Creates: Dev_Druid-X.Y.iso

############################## Cleanup

To clean up the source directory:

	$ make clean

This leaves your compiled ISO around. Only intermediary dependencies are
cleaned up.

To return the source directory to its original state:

	$ make distclean

This cleans up everything (including compiled ISOs). It is as if you have un-
packed a fresh source archive.


############################## Common Problems and Solutions

On some systems, you may get an error similar to the following at the very end
of the build:

dep/isohybrid.pl -offset 1 -h 255 -s 63 "Druid-X.Y.iso"
Can't locate bytes.pm in @INC (@INC contains: ...) at dep/isohybrid.pl line 20.
BEGIN failed--compilation aborted at dep/isohybrid.pl line 20.
gmake: *** [Druid-X.Y.iso] Error 2

You can safely ignore this message if-and-only-if you have no desire to write
the resulting ISO file to USB flash media or Hard Disk. You can still use the
ISO file produced on CD/DVD media.

The nature of the above problem is that the Druid build system attempts to
post-process the ISO file with the "isohybrid.pl" utility, allowing you to
write the contents of the ISO file to USB flash media or Hard Disk rather than
CD/DVD media. The above error occurs when the perl library "Bytes.pm" is not
installed, as this is a dependency of the "isohybrid.pl" utility.

To disable the use of the "isohybrid.pl" utility, you can optionally disable
its use by the Druid build system by executing:

	./configure --disable-isohybrid

Before continuing your normal build procedure.

################################################################################
#
# $Header: /cvsroot/druidbsd/druidbsd/druid83/doc/BUILD,v 1.1 2012/10/10 23:30:07 devinteske Exp $
#
# $Copyright: 2006-2012 Devin Teske. All rights reserved. $
#
# $Log: BUILD,v $
# Revision 1.1  2012/10/10 23:30:07  devinteske
# Commit initial public beta release of 8.3 series (beta 57; on-par with
# the 9.0 series released recently, also beta 57).
#
#
################################################################################
