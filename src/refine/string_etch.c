/*   string_etch.c                Version 1.6     */
/*   Last Modification : 8/29/94 10:53:09        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/

/*----------------------------------------------------------------------
**  Copyright 1990 by
**  The Board of Trustees of the Leland Stanford Junior University
**  All rights reserved.
**
**  This routine may not be used without the prior written consent of
**  the Board of Trustees of the Leland Stanford University.
**----------------------------------------------------------------------
**/

/*	string_etch.c	
 *	goodwin chin
 */

#include <stdio.h>
#include <malloc.h>

#include "constant.h"
#include "material.h"
#include "refine.h"
#include "global.h"
#include "misc.h"

#define BUFFERSIZE 80

void string_etch(
char *infile,
int material)
{
    char *buffer;
    int start_index;
    int stop_index;
    int nump, maxp;
    float xcord;
    float ycord;
    float *p[MAXDIM];
  
    FILE *inf;
  
    if ((buffer = (char *) malloc(BUFFERSIZE)) == NULL)
	panic("can't malloc buffer in string_etch\n");
 
    if ((inf = fopen(infile, "r")) == NULL)  
	panic("can't open for read in string_etch\n");
    else  {
	start_index = 0;

	/* read in data */
	/* get first point */
	fgets(buffer, BUFFERSIZE, inf);
        sscanf(buffer, "%f %f", &xcord, &ycord);

	maxp = 300;
	p[0] = salloc(float, maxp);
	p[1] = salloc(float, maxp);
	p[0][0] = p[0][1] = xcord * 1.0e-4;
	p[1][0] = (ycord - 1.0) * 1.0e-4;
	p[1][1] = ycord * 1.0e-4;
	stop_index = 2;

	while ((buffer = fgets(buffer, BUFFERSIZE, inf)) != NULL)  {
	    sscanf(buffer, "%f %f", &xcord, &ycord);
	    if ( stop_index >= maxp ) {
		maxp += 300;
		p[0] = sralloc(float, maxp, p[0]);
		p[1] = sralloc(float, maxp, p[1]);
	    }
	    p[0][stop_index] = xcord * 1.0e-4;
	    p[1][stop_index] = ycord * 1.0e-4;
	    stop_index++;
	}
	free((char *)buffer);

	nump = stop_index - start_index + 1;
	/* back up since we want last point again */
	p[0][nump - 1] = p[0][nump - 2];
	p[1][nump - 1] = p[1][nump - 2] - 1.0e-4;
	etch(p, nump, material);
	fclose(inf);
    }
    free((char *)p[0]);
    free((char *)p[1]);
}





