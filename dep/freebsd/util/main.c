/* -*- tab-width:  4 -*- ;; Emacs  */
/* vi: set tabstop=4     :: Vi/ViM */

/* Devin Teske (c)2006, April 19, 09:21:12. All Rights Reserved. */

/* custom includes (for all) */
#include "main.h"		/* configuration macros */

/* system includes (for all) */
#include <stdio.h>		/* printf() fflush() sprintf() */
#include <sys/sysctl.h>	/* sysctl() sysctlbyname() */
   /* strrchr() and strncmp() included by <string.h> through "main.h" */

/* system includes (for `htt', `sleep', `physmem', and `x86_64') */
#include <stdlib.h>		/* malloc() EXIT_SUCCESS EXIT_FAILURE strtoXX()
                   		 * exit() */

/* system includes (for `htt') */
#include <err.h>		/* err() */
#include <sys/syslog.h>	/* LOG_FAC() LOG_KERN */

/* system includes (for `x86_64') */
#include <machine/cpufunc.h>	/* read_eflags() write_eflags() do_cpuid() */
#include <machine/psl.h>		/* PSL_ID */
#include <machine/specialreg.h>	/* AMDID_LM */

/* system includes (for `sleep') */
#include <ctype.h>		/* isspace() isdigit() */
#include <limits.h>		/* INT_MAX */
#include <time.h>		/* timespec nanosleep() */
#include <unistd.h>		/* getopt() */

/* function prototypes */
int do_htt     (int argc, char *argv[]);
int do_swapsz  (int argc, char *argv[]);
int do_sleep   (int argc, char *argv[]);
int do_physmem (int argc, char *argv[]);
int do_x86_64  (int argc, char *argv[]);

/* function mappings */
struct { char *rname; int (*fn)(int,char**); } strfn[] = {
   { "htt",		do_htt		},
   { "swapsz",	do_swapsz	},
   { "sleep",	do_sleep	},
   { "physmem",	do_physmem	},
   { "x86_64",	do_x86_64	},
   { 0, 0 }
};

int
main (int argc, char *argv[])
{
   char *root_name;
   int n = 0;

   /* get the root name of our invocation path */
   root_name = strrchr(argv[0], '/');
   root_name = root_name ? ++root_name : argv[0];

   /* execute the proper `main' function given our root name */
   while (strfn[n].rname)
   {
      if (!strcmp(root_name, strfn[n].rname))
      {
         int err = strfn[n].fn(argc,argv);
         exit(err);
      }
      n++;
   }

   exit(EXIT_FAILURE);
}

int
do_htt (int argc, char *argv[])
{
   int ch, newl, skip;
   char *p, *ep, *bp;
   char buf[SEARCH_LEN + 8 + 1];
   int all = 0, pri = 0;
   size_t buflen;
   int bufpos;

   /* Running kernel. Use sysctl. */
   if (sysctlbyname("kern.msgbuf", NULL, &buflen, NULL, 0) == -1)
      err(1, "sysctl kern.msgbuf");
   if ((bp = malloc(buflen)) == NULL)
      errx(1, "malloc failed");
   if (sysctlbyname("kern.msgbuf", bp, &buflen, NULL, 0) == -1)
      err(1, "sysctl kern.msgbuf");

   memset(buf, 0, sizeof(buf));
   bufpos = 0;

   /*
    * The message buffer is circular.  If the buffer has wrapped, the
    * write pointer points to the oldest data.  Otherwise, the write
    * pointer points to \0's following the data.  Read the entire
    * buffer starting at the write pointer and ignore nulls so that
    * we effectively start at the oldest data.
    */
   p = bp;
   ep = bp + buflen;
   newl = skip = 0;
   do {
      if (p == bp + buflen)
         p = bp;
      ch = *p;
      /* Skip "\n<.*>" syslog sequences. */
      if (skip) {
         if (ch == '\n') {
            skip = 0;
            newl = 1;
         } if (ch == '>') {
            if (LOG_FAC(pri) == LOG_KERN || all)
               newl = skip = 0;
         } else if (ch >= '0' && ch <= '9') {
            pri *= 10;
            pri += ch - '0';
         }
         continue;
      }
      if (newl && ch == '<') {
         pri = 0;
         skip = 1;
         continue;
      }
      if (ch == '\0')
         continue;
      newl = ch == '\n';
      if (ch == '\n') {
         memset(buf, 0, sizeof(buf));
         bufpos = 0;
      } else if (bufpos < SEARCH_LEN + 8) {
         buf[bufpos++] = ch;
         if ((bufpos == SEARCH_LEN + 8) && !strncmp(buf, SEARCH, SEARCH_LEN)) {
            unsigned long features;
            features = strtoul((char *)(buf + SEARCH_LEN),0,16);
            if (features & HTT_FLAG)
               printf("HyperThreading Status: YES\n");
            else
               printf("HyperThreading Status: NO\n");
            return(EXIT_SUCCESS);
         }
      }
   } while (++p != ep);

   printf("HyperThreading Status: ERROR\n");
   return(EXIT_FAILURE);
}

int
do_swapsz (int argc, char *argv[])
{
   int mem = 0;
   size_t len = sizeof(mem);
   int selection[2] = { CTL_HW, HW_PHYSMEM };

   sysctl(selection, 2, &mem, &len, NULL, 0);
   printf("%i\n", ( mem / 512 ) * 2);
   return(EXIT_SUCCESS);
}

int
do_sleep (int argc, char *argv[])
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

int
do_physmem (int argc, char *argv[])
{
#ifdef HW_PHYSMEM
	size_t physmem = 0;
	size_t len = sizeof (physmem);
	static int mib[2] = { CTL_HW, HW_PHYSMEM };

#ifdef HW_PHYSMEM64
	if (len == 8) mib[1] = HW_PHYSMEM64;
#endif

	if (!sysctl(mib, 2, &physmem, &len, NULL, 0)
		&& len == sizeof (physmem))
	{
		printf("%u\n", physmem);
		return(EXIT_SUCCESS);
	}
#endif

	printf("-1\n");
	return(EXIT_FAILURE);
}

int
do_x86_64 (int argc, char *argv[])
{
	int has_lm = 0;
	char *cpu_vendor;
	int vendor[3];
	u_int eflags, regs[4];

	/* Check for presence of "cpuid". */
	eflags = read_eflags();
	write_eflags(eflags ^ PSL_ID);
	if (!((eflags ^ read_eflags()) & PSL_ID))
	{
		/* Fetch the vendor string. */
		do_cpuid(0, regs);
		vendor[0] = regs[1];
		vendor[1] = regs[3];
		vendor[2] = regs[2];
		cpu_vendor = (char *)vendor;

		/* check for vendors that support AMD features. */
		if (strncmp(cpu_vendor, "GenuineIntel", 12) != 0 &&
		    strncmp(cpu_vendor, "AuthenticAMD", 12) != 0)
		{
			/* Has to support AMD features. */
			do_cpuid(0x80000000, regs);
			has_lm = (regs[3] & AMDID_LM);
		}
	}

	printf("x86_64 support: %s\n", has_lm ? "YES" : "NO" );
	exit(EXIT_SUCCESS);
}


/*
 * $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/util/main.c,v 1.1 2012/10/10 23:30:05 devinteske Exp $
 *
 * $Copyright: 2006-2012 Devin Teske. All rights reserved. $
 *
 * $Log: main.c,v $
 * Revision 1.1  2012/10/10 23:30:05  devinteske
 * Commit initial public beta release of 8.3 series (beta 57; on-par with
 * the 9.0 series released recently, also beta 57).
 *
 *
 */
