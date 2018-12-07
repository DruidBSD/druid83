/* -*- tab-width:  4 -*- ;; Emacs  */
/* vi: set tabstop=4     :: Vi/ViM */

/* Devin Teske (c)2006, April 19, 09:24:04. All Rights Reserved. */

#ifndef HAVE_MAIN_H
#define HAVE_MAIN_H

/* system includes */
#include <sys/types.h>	/* standard types */
#include <errno.h>	/* errno */
#include <string.h>	/* strerror() */

/* Preprocessor Macros for `htt' */

   /* dmesg `Features' string to search for (from beginning of line) */
   #define SEARCH	"  Features=0x"
   #define SEARCH_LEN	13

   /* CPUID features bit signifying HyperThreading support */
   #define HTT_FLAG	0x10000000

/* Preprocessor Macros for `swapsz' */

   /* multiplier for determining amount of swap space by physical memory */
   #define SWAPSIZE	2

#endif /* HAVE_MAIN_H */

/*
 * $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/util/main.h,v 1.1 2012/10/10 23:30:05 devinteske Exp $
 *
 * $Copyright: 2006-2012 Devin Teske. All rights reserved. $
 *
 * $Log: main.h,v $
 * Revision 1.1  2012/10/10 23:30:05  devinteske
 * Commit initial public beta release of 8.3 series (beta 57; on-par with
 * the 9.0 series released recently, also beta 57).
 *
 *
 */
