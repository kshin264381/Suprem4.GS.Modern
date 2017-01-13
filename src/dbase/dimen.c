/*   dimen.c                Version 1.5     */
/*   Last Modification : 7/21/94 15:42:26        */
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

#include "global.h"
#include "constant.h"
#include "impurity.h"
#include "geom.h"
#include "misc.h"

/************************************************************************
 *									*
 *	set_dim( dim ) - This routine initiailizes the dimensionality	*
 *									*
 *  Original:	MEL	10/84						*
 *									*
 ************************************************************************/
void set_dim(int dim)
{
    switch( dim ) {
    case 1 :
       mode = ONED;
       nvrt = 2;
       nedg = 2;
       oxide_model = O_VERTICAL;
       break;
    case 2 :
       mode = TWOD;
       nvrt = 3;
       nedg = 3;
       break;
    default :
	panic("three dimensions are net yet supported");
    }
}
