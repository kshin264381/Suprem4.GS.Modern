/*   vert.c                Version 1.5     */
/*   Last Modification : 7/21/94 22:12:59        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine May not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#include "global.h"
#include "constant.h"
#include "geom.h"
#include "dbase.h"
#include "material.h"
#include "impurity.h"
#include "diffuse.h"		/* for noni */
#include "oxide.h"
#include "misc.h"

int dofxcmp(const void *aa, const void *bb);


/*-----------------VERT_GROWTH------------------------------------------
 * Try the assumption of vertical growth.
 * CSR Mar 86
 *----------------------------------------------------------------------*/
void vert_growth (
    float temp)		/* Processing temperature */
{
    float *dofx;	/* List of dx(x) */
    float vel[2];		/* velocity at a point */

    int nrec = 1;		/* Number of interface nodes recorded */
    int n, p, jn, i, j;
    int vs = (mode == ONED)?(imptosol[XVEL]):(imptosol[YVEL]);
    float x, xl, xh, vl, vh, dx1, dx2, dxdt; 

    /* Compute dx/dt at each interface node */
    dofx = salloc( float, 2 * nn );

    /* Loop over oxide nodes */
    for (n = 0; n < nn; n++) {
	if (nd[ n]->mater != SiO2) continue;

	/* See is there a Si node lurking underneath */
	p = nd[ n]->pt;
	if ((node_mat (pt[ p]->nd[ 0], Si)) < 0) continue;

	/* Compute the dxdt here */
	Onvel( n, vel);
	dxdt = (mode == ONED)?(vel[0]):(vel[1]);

	/* Store away in interface vector */
	dofx[ 2*nrec + 1] = dxdt;
	if ( mode == ONED )
	    dofx[ 2*nrec + 0] = 0.0;
	else
	    dofx[ 2*nrec + 0] = pt[ p]->cord[0];
	nrec++;
    }

    /* Sort the vector for interpolation */
    if ( nrec > 2 )
	qsort (&dofx[2], nrec-1, 2*sizeof(float), dofxcmp);

    /* Add two extra elements to handle the edges in a simple way */
    dofx[ 2*0 + 0] = -MAXFLOAT;
    dofx[ 2*0 + 1] = dofx[ 2*1 + 1];
    dofx[ 2*nrec + 0] = MAXFLOAT;
    dofx[ 2*nrec + 1] = dofx[ 2*(nrec-1) + 1];
    nrec++;

    /* Move points */
    for (p = 0; p < np; p++) {

	/* Straight-line interpolate displacement based on lateral position */
	if ( mode == ONED )
	    x = 0.0;
	else
	    x = pt[ p]->cord[0];

	for (i = 0; i < nrec; i++)
	    if (dofx[ 2*i + 0] > x) break;
	
	xl = dofx[ 2*(i-1) + 0];
	xh = dofx[   2*i + 0];
	vl = dofx[ 2*(i-1) + 1];
	vh = dofx[ 2*i + 1];

	dx1 = vl + (vh - vl) * (x - xl) / (xh - xl);
	dx2 = dx1 / alpha[SiO2][Si];

	/* 
	 * Compute displacements of all nodes at this location.
	 * Si3N4 nodes move with the SiO2 but Si nodes move 
	 * in the opposite direction, if they are on the boundary.
	 */
	for (j = 0; j < pt[ p]->nn; j++) {
	    jn = pt[ p]->nd[ j];
	    if (nd[ jn]->mater != Si) {
		nd[ jn]->sol[ vs] = (dx2 - dx1);
	    }
	    else if (node_mat( jn, SiO2) != -1) {
		nd[ jn]->sol[ vs] = dx2;
	    }
	}
    }
    free((char *)dofx);
}

int dofxcmp(const void *aa, const void *bb)
{
 float *a = (float *) aa;
 float *b = (float *) bb;
 
    if      (*a <  *b) return(-1);
    else if (*a == *b) return( 0);
    else               return( 1);
}

