/*   imp_qeqv.c                Version 1.4     */
/*   Last Modification : 7/21/94 15:57:31        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
/* date: 05 jul 85 (mje)
 *
 * "qeqv" : Find the equivalent dose through a given depth.
 *
 * calling sequence:
 *	double
 *	qeqv(xdep, dx, kp)
 *
 * where:
 *	<return>	- (double) Dose through 'xdep'.
 *	xdep		- (double) Depth through which to calculate dose.
 *	dx		- (double) Delta x to use in calculations.
 *	kp		- (double []) Pearson constants.
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
qeqv(
double	 xst, double xen ,
double	 qtot ,		/* total possible dose */
double	 dx ,
struct imp_info	 *data )
{

    double	 x , val, ov ;
    double	 q = 0.0;

    /*integrate from x = 0 until we get the correct dose*/
    ov = imp_vert(xst, data);
    for (x = xst + dx; (x < xen - dx) && (x < data->maxz); x += dx) {
	val = imp_vert( x, data );
	q += 0.5 * (val + ov) * dx ;
	ov = val;
    }

    /*add the excess for the last bit*/
    if ( x > (xen - dx) ) {
	val = imp_vert( xen, data );
	q += 0.5 * (val + ov) * (xen - x);
    }

    q =  q  * 1e-4 ;
    return ( q * qtot / data->area ) ;
}
