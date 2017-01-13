/*   norm.c                Version 1.5     */
/*   Last Modification : 7/21/94 15:51:24        */
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
#include <math.h>

#include "misc.h"

/************************************************************************
 *									*
 *	norm( arr, n ) - This routine calculates the norm of the array	*
 *  of doubles arr with length n.  Initially, it is set up to handle 	*
 *  a single norm, the two norm.  Later, it should be configured	*
 *  to do several different norms based on a passed parameter.		*
 *									*
 *  Original:	MEL	12/84						*
 *									*
 ************************************************************************/
double norm(
double *arr,
int n)
{
    double f;
    int i;

    f = 0.0;

    for(i = 0; i < n; i++)  {
	f += arr[i] * arr[i];
    }

    f = sqrt( f / n );
    return( f );
}
    

