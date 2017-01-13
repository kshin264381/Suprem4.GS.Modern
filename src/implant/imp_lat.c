/*   imp_lat.c                Version 1.4     */
/*   Last Modification : 7/21/94 15:57:30        */
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
 *     Copyright c 1991 The board of regents of the University of 	 *
 *                      Florida.  All rights reserved.			 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of the University of 	 *
 *     Florida.								 *
 *									 *
 *									 *
 *************************************************************************/
#include <stdio.h>
#include <math.h>
#include "global.h"
#include "constant.h"
#include "implant.h"

/************************************************************************
 *									*
 *	imp_latr( pos, left, right, data ) - This routine calculates 	*
 *  the lateral distribution function for lateral standard deviation 	*
 *  side and distance from the right and left sides of the slice.	*
 *									*
 *  Original:	MEL	1/87						*
 *									*
 ************************************************************************/
double imp_latr(
double pos,
struct imp_info *data)
{
    double siglat, val;

    val = exp( - (pos * pos) / (2.0 * data->lat[LATSIG] * data->lat[LATSIG]) );
	val /= data->lat[LATAREA] * 1.0e-4;
	return(val);
}
