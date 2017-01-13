/*   imp_zeqv.c                Version 1.4     */
/*   Last Modification : 7/21/94 15:57:34        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
/* date: 05 jul 85 (mje)
 *
 * "zeqv" : Find the equivalent dose through a given depth.
 *
 * calling sequence:
 *	double
 *	zeqv(dose, dx, kp)
 *
 * where:
 *	<return>	- (double) depth over which Dose extends.
 *	dose		- (double) Dose over which to find an 'zeqiv'
 *	dx		- (double) Delta x to use in calculations.
 *	kp		- (double []) Pearson constants.
 *
 * notes:
 *	> What happens if the target dose is too large for the implantation
 *	  parameters?
 *
 * written: Michael Eldredge (apr 85)
 *
 */

#include <math.h>
#define  abs(V) ((V) < 0.? -(V) : (V))

/* Offsets to the Pearson-IV constants array */
#include "global.h"
#include "constant.h"
#include "implant.h"

double
zeqv(
	double	 dose ,
	double	 qtot ,
	double	 dx ,
	double bt,
	struct imp_info	 *data )
	{

	double	 x , val, ov ;
	double	 q = 0.0 ;	/* Accumulating dose */
	double	 qtarg ;	/* target dose */

	qtarg = (dose / qtot) * data->area / dx * 1e4 ;

	ov = imp_vert(0.0, data);
	for (x = dx ; x < data->maxz ; x += dx) {
		val = imp_vert( x , data );

		q += 0.5 * (val + ov) ;
		if (q >= qtarg) {
		    x -= dx * (q - qtarg) / val ;
			break ;
			}
		ov = val;
		}

	return ( x ) ;
	}
