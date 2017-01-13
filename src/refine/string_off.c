/*   string_off.c                Version 1.5     */
/*   Last Modification : 8/29/94 10:53:31        */
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

/*	string_offset.c	
 *	goodwin chin
 */

#include <stdio.h>
#include <malloc.h>

#include "global.h"
#include "constant.h"
#include "refine.h"
#include "misc.h"
#define BUFFERSIZE 80

int string_offset(
char *infile,
struct line **poffset_array)
{
    char *buffer;
    int nump;
    float xcord;
    float ycord;
    struct line *offset_array;
    int maxop;
    FILE *inf;
  
    maxop = 100;
    *poffset_array = salloc( struct line, maxop );
    offset_array = *poffset_array;

  
    if ((buffer = (char *) malloc(BUFFERSIZE)) == NULL)
	panic("can't malloc space for buffer\n");
 
    if ((inf = fopen(infile, "r")) == NULL)  
	panic("can't open for read in gen_file_offset\n");
    else  {

	/* read in data */
	nump = 0;

	while ((buffer = fgets(buffer, BUFFERSIZE, inf)) != NULL)  {
	    sscanf(buffer, "%f %f", &xcord, &ycord);
	    offset_array[nump].x = xcord * 1.0e-4;
	    offset_array[nump].y = ycord * 1.0e-4;
	    offset_array[nump].map = 0;
	    offset_array[nump].p = -1;
	    nump++;
	    if ( nump == maxop ) {
		maxop += 100;
		*poffset_array = sralloc( struct line, maxop, offset_array );
		offset_array = *poffset_array;
	    }
	}
	free((char *)buffer);
	fclose(inf);
    }
    return(nump);
}

