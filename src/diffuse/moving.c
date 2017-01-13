/*   moving.c                Version 1.6     */
/*   Last Modification : 7/21/94 15:51:23        */
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
#include "global.h"
#include "constant.h"
#include "geom.h"
#include "diffuse.h"
#include "material.h"
#include "impurity.h"
#include "matrix.h"
#include "misc.h"




/************************************************************************
 *									*
 *	moving_bound() - this routine calculates the fluxes associated  *
 *  with the boundary motion.						*
 *									*
 *  Original:	MEL	4/86						*
 *									*
 ************************************************************************/
void moving_bound(
double dt,			
double scale)
{
    int i, j, k, ix, it, t, s;
    int nx1, nx2;
    int ysol, xsol, dsol;
    double oldln[2], midln[2], newln[2];
    int head, tail, hdnd, tlnd, nbr;
    double sci = 1.0 - scale;

    /*we only do this for regular impurities*/
    ysol = imptosol[YVEL];
    xsol = imptosol[XVEL];
    dsol = imptosol[DELA];

    if ( dsol == -1 ) return;

    /*initialize the arrays*/
    for(i = 0; i < nn; i++) old[dsol][i] = mid[dsol][i] = new_sol[dsol][i] = 0.0;

    /*once for each point node pair*/
    for(i = 0; i < np; i++) {
	for(j = 0; j < pt[i]->nn; j++) {
	    for(k = j + 1; k < pt[i]->nn; k++) {

		/*get the node and arrays into easy access*/
		nx1 = pt[i]->nd[j];
		nx2 = pt[i]->nd[k];

		/*if the dot product of the velocities is negative - growing interface*/
		if ( ((nd[nx1]->sol[ysol] * nd[nx2]->sol[ysol]) +
		     (nd[nx1]->sol[xsol] * nd[nx2]->sol[xsol])) < 0.0) {

		    /*make nx2 be the SiO2 material*/
		    if ( nd[nx1]->mater == SiO2 ) { ix = nx1;  nx1 = nx2;  nx2 = ix; }

		    switch(mode) {
		    case TWOD :
			/*zero the normal keepers*/
			oldln[0] = midln[0] = newln[0] = 0.0;
			oldln[1] = midln[1] = newln[1] = 0.0;

			/*find the interface normal pieces*/
			for( it = 0; it < num_tri_nd(nx1); it++ ) {
			    t = tri_nd(nx1, it);
			    for(s = 0; s < 3; s++) {

				/*only want interfaces with SiO2*/
				nbr = tri[t]->nb[s];
				if (nbr<0 || 
				    mat_reg(reg_tri(nbr)) != SiO2) continue;

				/*get the interface vectors*/
				hdnd = tri[t]->nd[(s+1)%3];
				tlnd = tri[t]->nd[(s+2)%3];
				head = nd[ hdnd ]->pt;
				tail = nd[ tlnd ]->pt;

				/*at old time...*/
				pt[head]->cord[0] -= new_sol[xsol][hdnd] * dt;
				pt[head]->cord[1] -= new_sol[ysol][hdnd] * dt;
				pt[tail]->cord[0] -= new_sol[xsol][tlnd] * dt;
				pt[tail]->cord[1] -= new_sol[ysol][tlnd] * dt;
				oldln[0] += pt[tail]->cord[1]-pt[head]->cord[1];
				oldln[1] += pt[head]->cord[0]-pt[tail]->cord[0];

				/*at mid time...*/
				pt[head]->cord[0] += scale*new_sol[xsol][hdnd]*dt;
				pt[head]->cord[1] += scale*new_sol[ysol][hdnd]*dt;
				pt[tail]->cord[0] += scale*new_sol[xsol][tlnd]*dt;
				pt[tail]->cord[1] += scale*new_sol[ysol][tlnd]*dt;
				midln[0] += pt[tail]->cord[1]-pt[head]->cord[1];
				midln[1] += pt[head]->cord[0]-pt[tail]->cord[0];

				/*at mid time...*/
				pt[head]->cord[0] += sci*new_sol[xsol][hdnd]*dt;
				pt[head]->cord[1] += sci*new_sol[ysol][hdnd]*dt;
				pt[tail]->cord[0] += sci*new_sol[xsol][tlnd]*dt;
				pt[tail]->cord[1] += sci*new_sol[ysol][tlnd]*dt;
				newln[0] += pt[tail]->cord[1]-pt[head]->cord[1];
				newln[1] += pt[head]->cord[0]-pt[tail]->cord[0];
			    }
			}

			/*compute the change in areas...*/
			old[dsol][nx1] = 0.5 * (old[xsol][nx1] * oldln[0] + 
						old[ysol][nx1] * oldln[1]);
			mid[dsol][nx1] = 0.5 * (mid[xsol][nx1] * midln[0] + 
						mid[ysol][nx1] * midln[1]);
			new_sol[dsol][nx1] = 0.5 * (new_sol[xsol][nx1] * newln[0] + 
						new_sol[ysol][nx1] * newln[1]);
			break;
		    
		    case ONED :
			old[dsol][nx1] = -old[xsol][nx1];
			mid[dsol][nx1] = -mid[xsol][nx1];
			new_sol[dsol][nx1] = -new_sol[xsol][nx1];
			break;
		    }
		}
	    }
	}
    }
}



