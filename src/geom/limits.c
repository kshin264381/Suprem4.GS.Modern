/*   limits.c                Version 1.4     */
/*   Last Modification : 7/21/94 15:54:00        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/

#include <stdio.h>
#include "global.h"
#include "constant.h"
#include "geom.h"


/************************************************************************
 *									*
 *	dev_lmts(dxmin, dxmax, dymin, dymax) - this routine calculates  *
 *  the limits of the device.						*
 *									*
 *    Original : CSR           Stanford University        Nov, 1983 	*
 *    Revision : MEL	       (C translation)		  Oct, 1984	*
 *									*
 ************************************************************************/
void dev_lmts(float *dxmin, float *dxmax, float *dymin, float *dymax)
{
    float *val;
    struct pt_str **p;

    /*initialize max and mins to first values**/
    *dxmin = *dxmax = pt[0]->cord[0];
    *dymin = *dymax = pt[0]->cord[1];

    /*loop through all the points*/
//    for( p = pt; *p != NULL; p += sizeof(pt[0] )) {
//	val = p[0]->cord;
//	*dxmin = (*dxmin < *val) ? *dxmin : *val;
//	*dxmax = (*dxmax > *val) ? *dxmax : *val;
//	val++;	/*advance to y coordinate*/
//	*dymin = (*dymin < *val) ? *dymin : *val;
//	*dymax = (*dymax > *val) ? *dymax : *val;
//    }

    int pt_array_size = sizeof(**pt)/sizeof(struct pt_str);
    int i;
    for( i=0; i < pt_array_size; i++ ) {
    	p = pt + i*sizeof(struct pt_str);
	val = p[0]->cord;
    	*dxmin = (*dxmin < *val) ? *dxmin : *val;
    	*dxmax = (*dxmax > *val) ? *dxmax : *val;
    	val++;	/*advance to y coordinate*/
    	*dymin = (*dymin < *val) ? *dymin : *val;
    	*dymax = (*dymax > *val) ? *dymax : *val;
    }
}


