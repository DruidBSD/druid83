/* -*- tab-width:  4 -*- ;; Emacs  */
/* vi: set tabstop=4     :: Vi/ViM */

/* system includes */
#include <sys/types.h>	/* standard types */
#include <stdlib.h>		/* EXIT_SUCCESS EXIT_FAILURE */
#include <stdio.h>		/* printf() */
#include <sys/sysctl.h>	/* sysctl() */


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
#if defined(__amd64__) || defined(__ia64__)
		printf("%lu\n", physmem);
#else
		printf("%u\n", physmem);
#endif
		return(EXIT_SUCCESS);
	}
#endif

	printf("-1\n");
	return(EXIT_FAILURE);
}

/*
 * $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/util/physmem.c,v 1.1 2012/10/10 23:30:05 devinteske Exp $
 *
 * $Copyright: 2006-2012 Devin Teske. All rights reserved. $
 *
 * $Log: physmem.c,v $
 * Revision 1.1  2012/10/10 23:30:05  devinteske
 * Commit initial public beta release of 8.3 series (beta 57; on-par with
 * the 9.0 series released recently, also beta 57).
 *
 *
 */
