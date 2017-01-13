/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   print_time.c                Version 5.1     */
/*   Last Modification : 7/3/91  08:41:12 */

#include <stdio.h>
#include <sys/param.h>
#include "global.h"
#include "shell.h"
/* couldn't find HZ on the CONVEX in an include file.  set it to
*    100 per Mike Eldredge
*/
#ifdef CONVEX
#define HZ 100
#endif

/* HZ for a 3100 is 60 per man page */
#ifndef HZ
#define HZ 60
#endif


/************************************************************************
 *									*
 *	print_time(name, before, after) - This routine prints timing	*
 *  information if the cpu flag has been set in a standard fashion.  	*
 *  The calling routine is responsible for having timed the routine 	*
 *  using time(3), and passing the resulting integer arrays along with	*
 *  a message string to this turkey.					*
 *									*
 *  Original:	MEL	11/84						*
 *									*
 ************************************************************************/
void print_time(
char *name,
int *before, int *after)
{
    if (cpufile != NULL) {
	fprintf(cpufile, "%-30s ", name);
	fprintf(cpufile, "user %8.6g\t",(after[0]-before[0])/(HZ*1.0));
	fprintf(cpufile, "system %8.6g\n", (after[1]-before[1])/(HZ*1.0));
    }
}

