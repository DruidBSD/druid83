/* -*- tab-width:  4 -*- ;; Emacs  */
/* vi: set tabstop=4     :: Vi/ViM */

/* Devin Teske (c)2006, September 28, 12:22:54. All Rights Reserved. */

/* system includes */
#include <unistd.h>   /* close() dup() sleep() execl() */
#include <fcntl.h>    /* open() O_RDWR */
#include <stdio.h>    /* printf() fflush() */
#include <stdlib.h>   /* EXIT_SUCCESS EXIT_FAILURE */

/* custom includes */
#include "main.h"     /* configuration macros */


/* For debugging purposes, this is a custom init used to delay the start of
 * another init program. Allowing the user time to press the scroll-lock
 * key and page up/down to view the kernel messages before the intended
 * program takes over the console (ie., since some init programs may take
 * over the console (such as sysinstall(8)).
 */
int main (int argc, char *argv[])
{
   int fd, n;

   /* close stdout and reopen it with output to tty */
   close(0);
   fd = open("/dev/ttyv0", O_RDWR);
   close(1); dup(0);

   /* output to console (just to let our users know what is happening) */
   printf("Starting %s", INIT_PATH);

   /* delay (allow time to block execution via scroll-lock) */
   for (n = INIT_DELAY; n > 0; n--)
   {
      printf(".");
      fflush(stdout);
      sleep(1);
   }

   /* execute init with */
   execl(INIT_PATH, INIT_PATH, 0);

   exit(EXIT_SUCCESS); /* should never reach here */
}

/*
 * $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/mfsroot/standard/dep/init/main.c,v 1.1 2012/10/10 23:29:58 devinteske Exp $
 *
 * $Copyright: 2006-2012 Devin Teske. All rights reserved. $
 *
 * $Log: main.c,v $
 * Revision 1.1  2012/10/10 23:29:58  devinteske
 * Commit initial public beta release of 8.3 series (beta 57; on-par with
 * the 9.0 series released recently, also beta 57).
 *
 *
 */
