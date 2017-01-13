/*   imp_qpgauss.c                Version 1.5     */
/*   Last Modification : 8/29/94 10:47:56        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
/* date: 5 jan 87 (mel)
 *
 * "qpgauss" :	return area under a gaussian curve
 *
 * written: Mark E. Law (jan 87)
 *
 */

#include <math.h>
#define  abs(V) ((V) < 0.? -(V): (V))
#include "global.h"
#include "constant.h"
#include "implant.h"

int qpgauss(
double	 dx ,
struct imp_info	 *cvals)	/* Pearson-IV constants) area) max Z*/
{

    double	 x;
    double  	 val = 1, v0;
    double	 qp = 0.0 ;

    /* --- start ---- */

    /*integrate from zero down to the bottom of the profile*/
    v0 = imp_vert(0.0, cvals);
    for (x = dx; (val > 1e-36) || (x < cvals->vert[GUS_RP]); x += dx) {
	 val = imp_vert(x , cvals);
	 qp += val + v0;
	 v0 = val;
    }

    /* The area under the Pearson. Also convert units since conc is
     *	atoms/cm^2  and dx is in microns.
     */
    cvals->area = 0.5 * qp * dx * 1e-4 ;	/* The total area */
    cvals->maxz = x;
    return(0);
}
