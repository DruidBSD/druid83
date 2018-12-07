/* -*- tab-width:  4 -*- ;; Emacs  */
/* vi: set tabstop=4     :: Vi/ViM */

/* Devin Teske (c)2006, August 21, 10:46:09. All Rights Reserved. */

/* system includes */
#include <sys/types.h>	/* standard types */
#include <stdlib.h>		/* EXIT_SUCCESS EXIT_FAILURE strtol() */
#include <ctype.h>		/* isspace() isdigit() */
#include <limits.h>		/* INT_MAX */
#include <time.h>		/* timespec nanosleep() */
#include <unistd.h>		/* getopt() */


int
main (int argc, char *argv[])
{
   struct timespec time_to_sleep;
   long l;
   int ch, neg;
   char *p;

   while ((ch = getopt(argc, argv, "")) != -1)
      switch(ch) {
      case '?':
      default:
         return(EXIT_FAILURE);
      }
   argc -= optind;
   argv += optind;

   if (argc != 1)
      return(EXIT_FAILURE);

   p = argv[0];

   /* Skip over leading whitespaces. */
   while (isspace((unsigned char)*p))
      ++p;

   /* Check for optional `+' or `-' sign. */
   neg = 0;
   if (*p == '-') {
      neg = 1;
      ++p;
   }
   else if (*p == '+')
      ++p;

   /* Calculate seconds. */
   if (isdigit((unsigned char)*p)) {
      l = strtol(p, &p, 10);
      if (l > INT_MAX) {
         /*
          * Avoid overflow when `seconds' is huge.  This assumes
          * that the maximum value for a time_t is >= INT_MAX.
          */
         l = INT_MAX;
      }
   } else
      l = 0;
   time_to_sleep.tv_sec = (time_t)l;

   /* Calculate nanoseconds. */
   time_to_sleep.tv_nsec = 0;

   if (*p == '.') {                /* Decimal point. */
      l = 100000000L;
      do {
         if (isdigit((unsigned char)*++p))
            time_to_sleep.tv_nsec += (*p - '0') * l;
         else
            break;
      } while (l /= 10);
   }

   if (!neg && (time_to_sleep.tv_sec > 0 || time_to_sleep.tv_nsec > 0))
      (void)nanosleep(&time_to_sleep, (struct timespec *)NULL);

   return(EXIT_SUCCESS);
}

/*
 * $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/util/sleep.c,v 1.1 2012/10/10 23:30:05 devinteske Exp $
 *
 * $Copyright: 2006-2012 Devin Teske. All rights reserved. $
 *
 * $Log: sleep.c,v $
 * Revision 1.1  2012/10/10 23:30:05  devinteske
 * Commit initial public beta release of 8.3 series (beta 57; on-par with
 * the 9.0 series released recently, also beta 57).
 *
 *
 */
