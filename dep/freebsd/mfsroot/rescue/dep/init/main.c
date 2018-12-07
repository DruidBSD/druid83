/* -*- tab-width:  4 -*- ;; Emacs  */
/* vi: set tabstop=4     :: Vi/ViM */

/* Devin Teske (c)2006, September 28, 12:22:54. All Rights Reserved. */

/* system includes */
#include <unistd.h>     /* close() dup() sleep() setsid() execl() */
#include <fcntl.h>      /* open() O_RDWR */
#include <stdio.h>      /* printf() fflush() strerror() setbuf() */
#include <stdlib.h>     /* EXIT_SUCCESS EXIT_FAILURE getenv() */
#include <paths.h>      /* _PATH_CONSOLE */
#include <errno.h>      /* errno */
#include <sys/ioctl.h>  /* ioctl() */

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

   /* XXX: Known bug...
    * We make no effort to provide support for TTY output on serial COM
    */

   setsid(); /* start new session */

   /* close stdout and reopen it with output to ttyv0 (Alt+F1) */
   close(0);
   fd = open("/dev/ttyv0", O_RDWR);
   if (fd == -1)
   {
      printf("Unable to open /dev/ttyv0: %s (%i)\n", strerror(errno), errno);
      printf("Falling back to %s\n", _PATH_CONSOLE);

      fd = open(_PATH_CONSOLE, O_RDWR); /* fallback */
      if (fd == -1)
      {
         printf("Unable to open %s: %s (%i)\n", _PATH_CONSOLE,
                strerror(errno), errno);
         _exit(1);
      }
   }

   /* redirect any output for fd==1 to ttyv0 */
   close(1); dup(0);

   /* close stderr and reopen it with output to ttyv1 (Alt+F2) */
   close(2);
   fd = open("/dev/ttyv1", O_WRONLY);
   if (fd != -1)
   {
      int on = 1;
      ioctl(fd, TIOCCONS, (char *)&on);
   }

   /* make ttyv0 the controlling terminal */
   ioctl(0, TIOCSCTTY, (char *)NULL);

   /* miscellaneous bootstrap (set login and disable buffering) */
   setlogin("root");
   setbuf(stdin, 0);
   setbuf(stderr, 0);

   /* It is now safe to write to the console in the standard mannerisms */

   /* Let our users know what is happening */
   printf("Starting %s", INIT_PATH);

   /* delay (allow time to block execution via scroll-lock) */
   for (n = INIT_DELAY; n > 0; n--)
   {
      printf(".");
      fflush(stdout);
      sleep(1);
   }
   printf("\n");

   /* execute init with */
   execl(INIT_PATH, INIT_PATH, (char *)0);

   exit(EXIT_SUCCESS); /* should never reach here */
}

/*
 * $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/mfsroot/rescue/dep/init/main.c,v 1.1 2012/10/10 23:29:57 devinteske Exp $
 *
 * $Copyright: 2006-2012 Devin Teske. All rights reserved. $
 *
 * $Log: main.c,v $
 * Revision 1.1  2012/10/10 23:29:57  devinteske
 * Commit initial public beta release of 8.3 series (beta 57; on-par with
 * the 9.0 series released recently, also beta 57).
 *
 *
 */
