/*   time.c                Version 1.8     */
/*   Last Modification : 8/31/94 14:25:17        */
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
#include <malloc.h>

#include "global.h"
#include "constant.h"
#include "geom.h"
#include "dbase.h"
#include "diffuse.h"
#include "impurity.h"
#include "matrix.h"
#include "misc.h"
#include "oxide.h"
#include "dislocation.h"

/************************************************************************
 *									*
 *	time_diff( delta, vel, temp, iter ) - This routine solves a 	*
 *  single timestep for the diffusion equation.  It solves at 		*
 *  temperature temp and velocity of the interface vector vel.  This 	*
 *  routine uses the Yeager timestep method.  It calculates the new 	*
 *  timestep and puts it into delta for the calling program to use in 	*
 *  synchronizing with oxide growth.  The main trick in this routine is *
 *  keeping the right vectors in the right locations.			*
 *									*
 *  Original:	MEL	1/86						*
 *									*
 ************************************************************************/
double timed_trbdf(
double delta,
float temp,
int compute)	/*flag for computing the comparison*/
{
    double del_t, old_t;	/*temp holder of the timestep*/
    double Kt;		/*the constant in the error term*/
    int xv= imptosol[XVEL], yv = imptosol[YVEL];
    int dela = imptosol[DELA];
    register int i, j, k, sk;	/*indices, what else?*/
    double *oarea;
    double *marea;
    double *narea;
    double nm;
    double tsave;
    double gam, igam;
    int fact;

    gam = 2.0 - sqrt( 2.0 );
    igam = 1.0 - gam;

    if ( tm_nsol == 0.0 ) return( 2.0 );

    if ( verbose >= V_NORMAL) printf("solving at update = %g\n", delta);

    oarea = salloc(double, (ned>nn)?(ned):(nn));
    marea = salloc(double, (ned>nn)?(ned):(nn));
    narea = salloc(double, (ned>nn)?(ned):(nn));

    del_t = delta * gam;
    tsave = total;

    /* save delta in del_t for calculation of loop growth in Isink() */
    loopdel_t = delta;

    /*compute the initial symbolic*/
    fact = upd_diff( 0.0, oarea, FALSE, TRUE );

    /*load the values*/
    for( j = 0; j < n_imp; j++ ) {
	if ( (!IS_DIFFUSE(soltoimp[j])) && (!IS_PSEUDO(soltoimp[j])) ) continue;
	if ( (IS_STEADY( soltoimp[j] )) && (!IS_LOCKED( soltoimp[j]) )) continue;
	for(i=0; i<nn; i++) new_sol[j][i] = mid[j][i] = old[j][i] = nd[i]->sol[j];
    }

    /*solve the diffusion problem if neccesary*/

    /*compute the initial right hand side*/
    do_vel(delta,gam);
    init_rhs( temp, old, olda, oarea );

    /*set up for the first TR step*/
    for(k = 0; k < tm_nsol; k++)  {
	sk = tm_sol[k];
	for(i = 0; i < nn; i++) oldb[sk][i] = newb[sk][i];
    }

    /*solve TR step to del_t*/
    if ( xv != -1 ) {
	/*update the grid locations*/
	fact = upd_diff( del_t, marea, FALSE, FALSE );
	do_vel(delta,gam);
	if (soldif_tr(del_t,temp,old,mid,olda,mida,oarea,marea,TRUE)) {
	    free((char *)oarea); free((char *)marea); free((char *)narea);
	    move_point( -del_t );
	    return( -1.0 );
	}
    }
    else {
	if (soldif_tr(del_t,temp,old,mid,olda,mida,oarea,oarea,TRUE)) {
	    free((char *)oarea); free((char *)marea); free((char *)narea);
	    return( -1.0 );
	}
    }

    /*set up for the BDF step*/
    total += del_t;
    old_t = del_t;
    del_t = igam * delta;
    for(k = 0; k < tm_nsol; k++) {
	sk = tm_sol[k];
	for(i = 0; i < nn; i++) new_sol[sk][i] = mid[sk][i];
    }

    /*now solve BDF2 for (1.0 - gam) * delta*/
    if ( xv != -1 ) {
	fact = upd_diff( del_t, narea, FALSE, FALSE );
	do_vel(delta,gam);
	if (soldif_bdf(del_t,old_t,temp,new_sol,mid,old,newa,mida,olda,
			narea, marea, oarea, fact) ) {
	    free((char *)oarea); free((char *)marea); free((char *)narea);
	    move_point( -delta );
	    return( -1.0 );
	}
    }
    else {
	if (soldif_bdf(del_t,old_t,temp,new_sol,mid,old,newa,mida,olda,
			oarea, oarea, oarea, FALSE) ) {
	    free((char *)oarea); free((char *)marea); free((char *)narea);
	    return( -1.0 );
	}
    }

    if ( compute ) {
	/*set up for a big step*/
	for(k = 0; k < tm_nsol; k++) {
	    sk = tm_sol[k];
	    for(i = 0; i < nn; i++) mid[sk][i] = new_sol[sk][i];
	}
	if ( xv != -1 ) {
	    for(i = 0; i < nn; i++) {
		mid[xv][i] = new_sol[xv][i];
		mid[yv][i] = new_sol[yv][i];
		mid[dela][i] = new_sol[dela][i];
	    }
	}

	/*Solve the big step*/
	if ( xv != -1 ) {
	    /*update the grid locations*/
	    i = soldif_tr(delta,temp,old,mid,olda,mida,oarea,narea,TRUE);
	}
	else {
	    i = soldif_tr(delta,temp,old,mid,olda,mida,oarea,oarea,TRUE);
	}


	/*now compute the norm of error for the new timestep*/
	for(nm = 0.0, k = 0; k < tm_nsol; k++) {
	    sk = tm_sol[k];
	    for(i = 0; i < nn; i++) {
		if ( oarea[i] != 0.0 ) {
		    Kt = (new_sol[sk][i] - mid[sk][i]) / 
			 (LTE[soltoimp[sk]] * new_sol[sk][i] + ABE[soltoimp[sk]]);
		    nm += Kt * Kt;
		}
	    }
	}

	total = tsave;
	nm = sqrt(nm / ( tm_nsol * nn ) );
    
	Kt = (nm > 0) ? exp( log( 60.0 / (9.0 * nm) ) / 3.0 ) : MAXFLOAT;
    }
    else {
	Kt = 1.0;
	for(nm = 0.0, k = 0; k < tm_nsol; k++) {
	    sk = tm_sol[k];
	    if ( !IS_LOCKED(soltoimp[sk]) ) 
		for(i = 0; i < nn; i++) nd[i]->sol[sk] = new_sol[sk][i];
	}
    }
    /*save the values, and continue*/
    for( j = 0; j < n_imp; j++ ) {
	if ( (!IS_DIFFUSE(soltoimp[j])) && (!IS_PSEUDO(soltoimp[j])) ) continue;
	if ( (IS_STEADY( soltoimp[j] )) && (!IS_LOCKED( soltoimp[j]) )) continue;
	for(i=0; i<nn; i++) nd[i]->sol[j] = new_sol[j][i];
    }

    /*total the areas up*/
    if ((verbose >= V_CHAT) && (xv != -1)) tot_areas( oarea, narea );

    /*save away the active concentrations*/
    for ( j = 0; j < n_imp; j++ ) {
	switch ( soltoimp[j] ) {
	case Asa   : i = As;   break;
	case Ba    : i = B;    break;
	case Sba   : i = Sb;   break;
	case Pa    : i = P;    break;
	case iBea  : i = iBe;  break;
	case iMga  : i = iMg;  break;
	case iSea  : i = iSe;  break;
	case iSia  : i = iSi;  break;
	case iSna  : i = iSn;  break;
	case iGea  : i = iGe;  break;
	case iZna  : i = iZn;  break;
	case iCa   : i = iC;   break;
	case iGa   : i = iG;   break;
	default    : i = 0;    break;
	}
	if (i) {
	    sk = imptosol[i];
	    for(i = 0; i < nn; i++) set_sol_nd(i, j, newa[sk][i]);
	}
    }

    /* save the new loop radius */
    if (loopr > 1.0e-7) {
        loopr = looprnew;

        /* These next 2 lines account for density changes */
        looplength = looplengthnew;
        rho = 0.5/(looplength*looplength);
        printf("timed_trbdf() new loop radius = %e\n", loopr);
        /* Use this if accounting for density changes */
        printf("timed_trbdf() new rho = %e\n", rho);
    }

    free((char *)oarea); free((char *)marea); free((char *)narea);
    return( Kt );
}


/*
 * update the velocities
 */
void do_vel(
double delta,
double gam)
{
    register int i;
    int xvst= MAXIMP - 5, yvst = MAXIMP - 6;
    int xv= imptosol[XVEL], yv = imptosol[YVEL];
    double igam = 1.0 - gam;

    if (xv == -1) return;

    /*set up the old velocities*/
    for(i = 0; i < nn; i++) {
	old[xv][i] = nd[i]->sol[xvst];
	old[yv][i] = nd[i]->sol[yvst];
	mid[xv][i] = igam*nd[i]->sol[xvst]+gam*nd[i]->sol[xv];
	mid[yv][i] = igam*nd[i]->sol[yvst]+gam*nd[i]->sol[yv];
	new_sol[xv][i] = nd[i]->sol[xv];
	new_sol[yv][i] = nd[i]->sol[yv];
    }

    /*use the velocity and location data to get the delta areas*/
    moving_bound( delta, gam );
}

