/* -*- tab-width:  4 -*- ;; Emacs  */
/* vi: set tabstop=4     :: Vi/ViM */

/* Devin Teske (c)2006, September 28, 12:23:18. All Rights Reserved. */

#ifndef HAVE_MAIN_H
#define HAVE_MAIN_H

/* system includes */
#include <sys/types.h>   /* standard types */
#include <errno.h>       /* errno */
#include <string.h>      /* strerror() */

/* Preprocessor Macros */

   /* configure the path to sysinstall */
   #define INIT_PATH     "/stand/sysinstall"

   /* delay (in seconds) before starting INIT_PATH */
   #define INIT_DELAY    3

#endif /* HAVE_MAIN_H */

/*
 * $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/mfsroot/standard/dep/init/main.h,v 1.1 2012/10/10 23:29:58 devinteske Exp $
 *
 * $Copyright: 2006-2012 Devin Teske. All rights reserved. $
 *
 * $Log: main.h,v $
 * Revision 1.1  2012/10/10 23:29:58  devinteske
 * Commit initial public beta release of 8.3 series (beta 57; on-par with
 * the 9.0 series released recently, also beta 57).
 *
 *
 */
