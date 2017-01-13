/*   bfill.c                Version 1.5     */
/*   Last Modification : 7/24/94 12:34:11        */
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
#include "dbaccess.h"
#include "skel.h"
#include "refine.h"
#include "dbase.h"


/*-----------------DVEDGE-----------------------------------------------
 * Divide an edge
 *----------------------------------------------------------------------*/
void div_edg(
int ie)
{
    double lej, ha, hb, rr, ff;
    int nn, inew, i;
    double updn = 10.0;
    float c[MAXDIM];

    lej = len_edg(ie);
    ha  = get_space(pt_edg(ie,0));
    hb  = get_space(pt_edg(ie,1));

    if ( (lej / ha > updn) && (lej / hb > updn) ) {
	/*split the edge in half, than treat the pieces...*/
	for(i = 0; i < mode; i++) 
	    c[i] = 0.5 * (cordinate(pt_edg(ie,0),i)+cordinate(pt_edg(ie,1),i));
	inew = sp_edge(ie, c, &nn, FALSE);
	ha  = get_space(pt_edg(ie,0));
	hb  = get_space(pt_edg(ie,1));
	dvpram(ha, hb, len_edg(ie), &nn, &rr, &ff);
	dvedge(ie, len_edg(ie), nn, rr, ff);

	ha  = get_space(pt_edg(inew,0));
	hb  = get_space(pt_edg(inew,1));
	dvpram(ha, hb, len_edg(inew), &nn, &rr, &ff);
	dvedge(inew, len_edg(inew), nn, rr, ff);
    }
    else {
	dvpram(ha, hb, lej, &nn, &rr, &ff);
	dvedge(ie, lej, nn, rr, ff);
    }
}




/*-----------------DVEDGE-----------------------------------------------
 * Divide an edge, using the spacing parameters calculated in dvpram.
 * Called from bfill, and also during the triangulation (triang.c).
 *----------------------------------------------------------------------*/
void dvedge (
int ie,			/* Edge to divide. */
double lej,		/* Length of edge. */
int nnew,		/* Number of new nodes to add */
double ratio,		/* Ratio of each interval to next. */
double first)		/* First interval as a fraction of lej. */
{
    int inew, ip, ied;

    double dx,dy,h,frac;
    float c[MAXDIM];
 
    /*if we don't need to add any, we don't need to add any*/
    if (nnew==0) return;

    c[0]  = cordinate( pt_edg(ie,0), 0);
    c[1]  = cordinate( pt_edg(ie,0), 1);
    dx = cordinate( pt_edg(ie,1), 0) - c[0];
    dy = cordinate( pt_edg(ie,1), 1) - c[1];
    frac = first;
    h =  frac*lej;
    if (ratio < 1) h *= ratio;

    for (inew=1; inew<=nnew; inew++) {

	c[0] += frac*dx;
	c[1] += frac*dy;
	
	ied = sp_edge(ie,c,&ip,FALSE); 
	set_space(ip, h);

	if (pt_edg(ie,0) != ip) ie = ied;

	h *= ratio;
	frac *= ratio;
    }

    return;
}
