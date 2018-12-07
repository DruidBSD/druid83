/* -*- tab-width:  4 -*- ;; Emacs  */
/* vi: set tabstop=4     :: Vi/ViM */

/* Devin Teske (c)2006, September 25, 16:12:36. All Rights Reserved. */

/* system includes */
#include <sys/types.h>	/* standard types */
#include <stdlib.h>		/* EXIT_SUCCESS */
#include <stdio.h>		/* printf() */
#include <sys/sysctl.h>	/* sysctl() */

/* Preprocessor Macros */
#define SWAPSIZE	2		/* multiplier times amount of RAM */
#define DEFAULTSWAP	1048576	/* sectors (512MB) */

int
main (int argc, char *argv[])
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
		printf("%u\n", ( physmem / 512 ) * SWAPSIZE);
		return(EXIT_SUCCESS);
	}
	else
		printf("%u\n", DEFAULTSWAP);
#else
	printf("%u\n", DEFAULTSWAP);
#endif

	return(EXIT_FAILURE);
}

/*
 * $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/util/swapsz.c,v 1.1 2012/10/10 23:30:05 devinteske Exp $
 *
 * $Copyright: 2006-2012 Devin Teske. All rights reserved. $
 *
 * $Log: swapsz.c,v $
 * Revision 1.1  2012/10/10 23:30:05  devinteske
 * Commit initial public beta release of 8.3 series (beta 57; on-par with
 * the 9.0 series released recently, also beta 57).
 *
 *
 */
