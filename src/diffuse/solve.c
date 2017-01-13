/*   solve.c                Version 1.6     */
/*   Last Modification : 7/21/94 15:51:28        */
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
#include <sys/param.h>
#include <sys/types.h>
#include <sys/times.h>

#include "global.h"
#include "constant.h"
#include "geom.h"
#include "diffuse.h"
#include "impurity.h"
#include "material.h"
#include "matrix.h"
#include "defect.h"
#include "misc.h"
#include "local_math.h"

/* HZ is 100 per Mike Eldredge on the Convex,
   which should be included in the file sys/param, above*/
#ifdef CONVEX
#define HZ 100
#endif
#ifdef HZ
#define HZ 60
#endif

#define MSIP 1.47334
#define BSIP 22.6746
#define MMGS -3.08542
#define MBGS 15.9161
#define BMGS -6.15469
#define BBGS 31.0333

/*the amount of over solve needed for this inner loop*/
#define NEWT 1.0




/************************************************************************
 *									*
 *	soldif_tr( del_t, temp, old, new, vel, iter ) -  This 		*
 *  routine gets the answer at a single time step.  It does this for 	*
 *  all the impurities and at the temp and velocity of the surface 	*
 *  indicated.	If the iter flag is true, it solves the linear system	*
 *  with an iterative method.						*
 *									*
 *  Original:	MEL	12/84						*
 *									*
 ************************************************************************/
int soldif_tr(
double del_t,		/*the amount of time step*/
float temp,		/*the temperature of the solve*/
double **old,		/*the answer of the previous timestep*/
double **nuevo,		/*the answer of this solution*/
double **olda,		/*the answer of the previous timestep*/
double **newa,		/*the answer of this solution*/
double *oldarea,	/*the area of the previous timestep*/
double *newarea,	/*the area of this solution*/
int init)		/*whether or not to reuse the last factorization*/
{

    /*compute the appropriate boundary condition elements*/
    bval_compute( temp, nuevo, del_t );

    /*load the setup structure*/
    cs.old_del = 0.0;
    cs.new_del = del_t;
    cs.temp = temp;
    cs.old = old;
    cs.nuevo = nuevo;
    cs.olda = olda;
    cs.newa = newa;
    cs.nco = newarea;
    cs.oco = oldarea;
    cs.type = TR;
    cs.sol = tm_sol;
    cs.nsol = tm_nsol;
    cs.elim = tm_elim;
    cs.il = tm_il;
    cs.l = tm_l;
    cs.loff = tm_loff;

    /*now call the real solve routine*/
    return( soldif( init, "TR", do_setup ) );
}


/************************************************************************
 *									*
 *	soldif_bdf( del_t, temp, old, new, vel, iter ) -  This routine	*
 *  solves a single step of the diffusion equation using BDF2.  If a	*
 *  negative concentrations are encountered, the routine returns true.	*
 *									*
 *  Original:	MEL	1/86						*
 *									*
 ************************************************************************/
int soldif_bdf(
double del_t,		/*the amount of time step*/
double old_t,		/*last time step*/
float temp,		/*the temperature of the solve*/
double **nuevo,		/*the answer of this solution*/
double **mid,		/*the answer of the previous timestep*/
double **old,		/*the answer of the really old timestep*/
double **newa,		/*the answer of this solution*/
double **mida,		/*the answer of the previous timestep*/
double **olda,		/*the answer of the really old timestep*/
double *newarea,	/*the area of this solution*/
double *midarea,	/*the area of the previous timestep*/
double *oldarea,	/*the area of the really old timestep*/
int init)		/*whether or not a setup has been done*/
{

    /*compute the appropriate boundary condition elements*/
    bval_compute( temp, nuevo, del_t );
	     
    /*load the setup structure*/
    cs.new_del = del_t;
    cs.old_del = old_t;
    cs.temp = temp;
    cs.old = old;
    cs.mid = mid;
    cs.nuevo = nuevo;
    cs.olda = olda;
    cs.mida = mida;
    cs.newa = newa;
    cs.nco = newarea;
    cs.mco = midarea;
    cs.oco = oldarea;
    cs.type = BDF;
    cs.sol = tm_sol;
    cs.nsol = tm_nsol;
    cs.elim = tm_elim;
    cs.il = tm_il;
    cs.l = tm_l;
    cs.loff = tm_loff;

    /*now call the real solve routine*/
    return( soldif( init , "BDF", do_setup) );
}




/************************************************************************
 *									*
 *	steady_state( ) - This routine calculates the steady state 	*
 *  variables.								*
 *									*
 *  Original:	MEL	11/85						*
 *									*
 ************************************************************************/
void steady_state(
float temp,
int nsol,
int *sol,
double **nuevo,
double *newarea)
{
    register int i, k, sk;
    int elim[MAXIMP];

    if ( nsol == 0 ) return;

    for(i = 0; i < nsol; i++) {
	if ( soltoimp[sol[i]] != Psi )
	    elim[i] = FALSE;
	else
	    elim[i] = TRUE;
    }

    /*load up the solution variables*/
    for(k = 0; k < nsol; k++) {
	sk = sol[k];
	for(i = 0; i < nn; i++) nuevo[sk][i] = nd[i]->sol[sk];
    }
    /*load up concentration variables for potential solution*/
    for(k = 0; k < n_imp; k++) {
		sk = soltoimp[k];
		switch( sk ) {
			case As :
			case B  :
			case Sb :
			case P  :
			case I  :
			case V  :
			case Psi:
			case iBe:
			case iMg:
			case iSe:
			case iSi:
			case iSn:
			case iGe:
			case iZn:
			case iC :
			case iG :
				for(i = 0; i < nn; i++) nuevo[k][i] = nd[i]->sol[k];
				break;
		}
    }

    /*compute the appropriate boundary condition elements*/
    bval_compute( temp, nuevo, 0.0 );
	     
    /*load the setup structure*/
    cs.old_del = 0.0;
    cs.new_del = 0.0;
    cs.temp = temp;
    cs.nuevo = nuevo;
    cs.newa = newa;
    cs.nco = newarea;
    cs.type = SS;
    cs.sol = sol;
    cs.nsol = nsol;
    cs.elim = elim;
    cs.il = ss_il;
    cs.l = ss_l;
    cs.loff = ss_loff;

    /*now call the real solve routine*/
    k = soldif( TRUE , "SS", do_setup );

    /*save away the calculated values*/
    for(k = 0; k < nsol; k++) {
    	sk = sol[k];
    	for(i = 0; i < nn; i++) nd[i]->sol[sk] = nuevo[sk][i];
    }

}



/************************************************************************
 *									*
 *	soldif() - This routine solves the equations as setup for which *
 *  ever method is appropriate.  It uses a newton loop and refactors 	*
 *  the jacobian when appropriate.					*
 *									*
 *  Original:	MEL	5/88						*
 *									*
 ************************************************************************/
int soldif( 
int init,
char *label,
void (* do_setup) (double *) )
{
    register int i, si, j;			/*standard flame about indices*/
    int converge = FALSE;		/*are we converged yet??*/
    int count, blk;				/*loop count*/
    struct tms before, after;		/*for the system time call*/
    double nm[MAXIMP], maxnorm; 	/*norms for the newton iteration*/
    float tset, tsol;			/*time for setup, time for solution*/
    double rhsnm[MAXIMP], rhs2, maxrhs;	/*two norm of the right hand side*/
    double scrhs, lstnm = 1.0e37;
    int negat = FALSE;
    int factor;
    double t1;
    double absrhserr = (mode==ONED)?(1.0e5):(10);
    char *ans = "  %c%-4d    %-11.5g  %-6.4g  %-6.4g  %-7.5g  %4d\n";
    char *s1  = "   iter    newton       setup   solve   ln rhs   Blk   (%s)\n";
    char *s2  = "           error        time    time    norm\n";

    if (cs.nsol == 0 ) return(FALSE);

    /*print the header if need be*/
    if ( verbose >= V_NORMAL) { printf(s1, label); printf(s2); }

    /*compute the initial matrix terms*/
    times(&before);
    do_setup( rhsnm );
    for(rhs2=0.0, i = 0; i < cs.nsol; i++) rhs2 += rhsnm[cs.sol[i]];
    rhs2 = sqrt( rhs2 );
    times(&after);
    tset = (after.tms_utime - before.tms_utime) / (HZ * 1.0);

    count = 0;
    while ( (! converge) && (!negat) ) {
	count++;
	
	times(&before);
	/*figure if we need to factor the matrix*/
	switch ( methdata.factor ) {
	case RF_ALL : factor = TRUE;
		      break;
	case RF_ERR : factor = lstnm < (rhs2 * 0.1);
		      break;
	case RF_TIM : factor = (count == 1) && init;
		      break;
	}

	/*solve it with the appropriate method*/
	solve_blocks(nn, cs.sol, cs.nsol, cs.elim, cs.il, cs.l, cs.loff, newb, factor );
	times(&after);
	tsol = (after.tms_utime - before.tms_utime) / (HZ * 1.0);

	/*compute the update dot products*/

	/*update the set of variables*/
	negat = update_vars( nm, cs.nsol, cs.sol, nn, cs.nuevo, newb );

	if ( negat ) continue;

	/*compute the initial matrix terms*/
	times(&before);
	do_setup( rhsnm );
	times(&after);
	tset = (after.tms_utime - before.tms_utime) / (HZ * 1.0);

	lstnm = rhs2;
	for(scrhs = rhs2 = 0.0, maxrhs = maxnorm = -1.0, i = 0; i < cs.nsol; i++)  {
	    si = cs.sol[i];
	    if ( nm[si] > maxnorm ) maxnorm = nm[si];
	    if ( rhsnm[si] > maxrhs ) {blk = soltoimp[si]; maxrhs = rhsnm[si]; }
	    rhs2 += rhsnm[si];
	    for(j = 0; j < nn; j++) {
		t1 = rhs[si][j] / (cs.nuevo[si][j] + 1.0);
		scrhs += t1 * t1;
	    }
	}
	rhs2 = sqrt( rhs2 );
	scrhs = sqrt( scrhs );

	if ( cs.type == SS )
	    converge = maxnorm < 1.0e-3;
	else
	    converge = (rhs2 < absrhserr) || (maxnorm*NEWT < 1.0e-6) ;

	/*check to make sure we are making a least a little progress*/
	if ( (count > 10) && (rhs2 / lstnm > 0.999) ) negat = TRUE;
	if ( ( rhs2 / lstnm > 1.0e10 ) ) negat = TRUE;
	if ( isnan( rhs2 ) ) negat = TRUE;

	/*print out the loop data*/
	if ( verbose >= V_NORMAL) {
	    if ( rhs2 == 0.0 )
		printf(ans, ' ', count, maxnorm*NEWT, tset, tsol, -10.0, blk);
	    else
		printf(ans, ' ', count, maxnorm*NEWT, tset, tsol, log10(rhs2), blk);
	    fflush(stdout);
	}

    }
    return( negat );
}





/************************************************************************
 *									*
 *	update_vars( ) - This routine takes the update vector and 	*
 *  corrects the current guess.  It also calculates the norm of the 	*
 *  the update.  If any updates take the concentration negative, the	*
 *  routine returns true.						*
 *									*
 *  Original:	MEL	11/85  (Older than this, subroutine date)	*
 *									*
 ************************************************************************/
int update_vars(
double *nm,
int nsol,
int *sol,
int nv,
double **vars,
double **update)
{
    int bi, si, j;		/*loop counters*/
    double tmp;
    int imp;
    int negat = FALSE;

    for(bi = 0; bi < nsol; bi++) {
		si = sol[bi];
		nm[si] = 0.0;
		imp = soltoimp[si];

		if (imp != Psi ) {
			for(j = 0; j < nv; j++) {
				/*calculate the value of the update*/
				tmp = vars[si][j] + update[si][j];

				/*check to see if the new conc will be negative*/
				// --> comparing floating point directly to numerical value..
				// is actually sooooooooo BAD.
				// --> implemented comparator function at local_math.h
				if ( d_lt(tmp, 0.0) ) {

					/*only sweat non pseudo vars*/
					if ( ! IS_PSEUDO( imp ) ) negat++;
		
					/*if the amount we are going negative is less than ABE*/
					if ( vars[si][j] > ABE[imp] )
					vars[si][j] = LTE[imp] * ABE[imp];
					else
					vars[si][j] = LTE[imp] * LTE[imp] * vars[si][j];

					tmp = update[si][j] / (LTE[imp] * vars[si][j] + ABE[imp]);
				}
				else {
					/*normal updating*/
					vars[si][j] = tmp;
					tmp = update[si][j] / (LTE[imp] * vars[si][j] + ABE[imp]);
				}
				/*norm calculation*/
				if ( d_mt(tmp, nm[si]) ) nm[si] = tmp;
				else if ( d_mt(-tmp, nm[si]) ) nm[si] = - tmp;
			}
		}
		else {
			/*damp the godammn poisson equations*/
			for(j = 0; j < nv; j++) {
				tmp = vars[si][j] + update[si][j];

				if ( d_lt(tmp, -1.0) ) tmp = -1.0;
				if ( d_mt(tmp, 1.0) ) tmp = 1.0;

				vars[si][j] = tmp;
				tmp = update[si][j] / (LTE[imp] * vars[si][j] + ABE[imp]);

				/*norm calculation*/
				if ( d_mt(tmp, nm[si]) )
					nm[si] = tmp;
				else if ( d_mt(-tmp, nm[si]))
					nm[si] = - tmp;
			}
		}
    }
    return( negat );
}



