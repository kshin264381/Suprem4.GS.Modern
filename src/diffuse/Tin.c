/*   Tin.c                Version 1.7     */
/*   Last Modification : 7/21/94 15:51:12        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
/*************************************************************************
 *									 *
 *     Copyright c 1991 The board of trustees of the Leland Stanford 	 *
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
#include "impurity.h"
#include "matrix.h"
#include "material.h"
#include "diffuse.h"
#include "misc.h"
/*definitions for impurity constants as a function of material*/
		 /*pre exp constant with neutral V*/
#define Dix0(M)  impur[iSn].constant[M][1][IS_IMPLANTED(iSn)]
		 /*exp constant with neutral V*/
#define DixE(M)  impur[iSn].constant[M][2][IS_IMPLANTED(iSn)]
		 /*pre exp constant with pos V*/
#define Dim0(M)  impur[iSn].constant[M][3][IS_IMPLANTED(iSn)]
		 /*exp constant with pos V*/
#define DimE(M)  impur[iSn].constant[M][4][IS_IMPLANTED(iSn)]
		 /*pre exp constant with 2x pos V*/
#define Dimm0(M) impur[iSn].constant[M][5][IS_IMPLANTED(iSn)]
		 /*exp constant with 2x pos V*/
#define DimmE(M) impur[iSn].constant[M][6][IS_IMPLANTED(iSn)]
		 /*fractional interstitialcy*/
#define Fi(M)    impur[iSn].constant[M][7][IS_IMPLANTED(iSn)]

/*definitions of the segregation coefficients as a function of material*/
#define Seg0(M1, M2) 	impur[iSn].seg[SEG0][M1][M2]
#define SegE(M1, M2) 	impur[iSn].seg[SEGE][M1][M2]
#define Trn0(M1, M2) 	impur[iSn].seg[TRN0][M1][M2]
#define TrnE(M1, M2) 	impur[iSn].seg[TRNE][M1][M2]


/* solid solubility data */
static double Snsolid[2][100];
static int Snssmax;
static double Snsstemp = 0.0;

/************************************************************************
 *									*
 *	This file contains definitions of the Tin routines.  It 	*
 *  includes the routine to calculate diffusion coefficients, the one	*
 *  to calculate any coupling terms between species, and the one to	*
 *  to calculate the boundary condition terms for the Tin.		*
 *  Provided the user does not want to change the parameters or access	*
 *  other data, they should be able to modify these extensively without *
 *  any harm to the convergence or solution techniques.			*
 *  									*
 *  Several data base access routines are defined in the file species.c	*
 *  to make it easier for the user to implement routines and get at	*
 *  other data.								*
 *									*
 ************************************************************************/


/************************************************************************
 *									*
 *	Sndiff_coeff(temp, noni, inter, vacan) - This routine returns	*
 *  the tin diffusivity as a function of temperature, n/ni, 		*
 *  interstitial and vacancy concentrations.				*
 *									*
 *  Original:	SEH	8/91   (Based on Arsenic.c by MEL)              *
 *									*
 ************************************************************************/
void Sndiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt)		/*the partial of the vacancy diffusivity*/
{
    register int i;
    int mat;
    double Dix[MAXMAT];
    double Dim[MAXMAT], Dimm[MAXMAT];
    double diff, part, Vt = kb * temp;

    /*first calculate the fixed stuff*/
    for(i = 0; i < MAXMAT; i++) {
	Dix[i]  = Dix0(i)  * exp ( - DixE(i)  / Vt );
	Dim[i]  = Dim0(i)  * exp ( - DimE(i)  / Vt );
	Dimm[i] = Dimm0(i) * exp ( - DimmE(i) / Vt );
    }

    for(i = 0; i < nn; i++) {
	mat = nd[i]->mater;

	/*calculate the total diffusivity*/
	diff = Dix[mat]
		+ (Dim[mat] + Dimm[mat] * noni[i]) * noni[i];
	part = (2.0 * Dimm[mat] * noni[i] + Dim[mat]) * noni[i] / Vt;

	idf[i] = Fi( mat ) * diff;
	vdf[i] = (1.0 - Fi( mat )) * diff;

	iprt[i] = Fi( mat ) * part;
	vprt[i] = (1.0 - Fi( mat )) * part;
    }

}


/************************************************************************
 *									*
 *	Snactive( conc, ans, temp ) - This routine calculates the 	*
 *  electrically active portion of the Tin concentration.		*
 *  This routine assumes that the material in question is GaAs.		*
 *									*
 *  Original:	SEH	8/91   (Based on Arsenic.c by MEL)              *
 *									*
 ************************************************************************/
void Snactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni)
{
    register int i;
    register int j;
    register int ss=imptosol[iSn];
    double f;
    double p, a, Snss, bSnss;
    double b = 0.90;
    double tmp;


    /*compute the solid solubility at this temperature*/
    Snsstemp = temp;
    for(i = 0; (i < Snssmax) && (Snsolid[0][i] < temp); i++);

    /*if we are at the low end*/
    if ( i == 0 ) {
	tmp = (temp - Snsolid[0][0]) / (Snsolid[0][1] - Snsolid[0][0]);
	Snss = tmp * (Snsolid[1][1] - Snsolid[1][0]) + Snsolid[1][0];
    }
    else if ( i == Snssmax ) {
	j = Snssmax - 1;
	tmp = (temp - Snsolid[0][j]) / (Snsolid[0][j-1] - Snsolid[0][j]);
	Snss = tmp * (Snsolid[1][j-1] - Snsolid[1][j]) + Snsolid[1][j];
    }
    else {
	tmp = (temp - Snsolid[0][i-1]) / (Snsolid[0][i] - Snsolid[0][i-1]);
	Snss = tmp * (Snsolid[1][i] - Snsolid[1][i-1]) + Snsolid[1][i-1];
    }

    f = 1.0;
    p = Snss * (1.0 - b); 
    a = Snss - p * log( p );
    bSnss = b * Snss;

    /*if simple, don't compute the appropriate derviatives*/
    if ( simple ) {
	/*repeat for all the nodes*/
	for(i = 0; i < nn; i++) {

	    if ( INSULATE( nd[i]->mater ) ) {
		act[ss][i] = conc[ss][i];
	    }
	    else {
		if ( conc[ss][i] > Snss ) {
		    act[ss][i] = a + p * log( conc[ss][i] - bSnss );
		    act[ss][i] *= f;
		}
		else {
		    act[ss][i] = f * conc[ss][i];
	 	}
	    }
	}
    }
    
    /*ok - compute the derivatives*/
    else {
	/*repeat for all the nodes*/
	for(i = 0; i < nn; i++) {

	    if ( INSULATE( nd[i]->mater ) ) {
	        act[ss][i] = conc[ss][i];
	        dact[ss][ss][i] = 1.0;
	    }
	    else {
		if ( conc[ss][i] > Snss ) {
		    act[ss][i] = a + p * log( conc[ss][i] - bSnss );
		    dact[ss][ss][i] = f * p / (conc[ss][i] - bSnss);
		    act[ss][i] *= f;
	    	}
		else {
	    	    act[ss][i] = f * conc[ss][i];
	    	    dact[ss][ss][i] = f;
		}
	    }
	}
    }
} 




/************************************************************************
 *									*
 *	Snboundary - This routine handles the computation of the	*
 *  tin boundary conditions at node i and solutions j and k.  The	*
 *  time step is delta and the temperature is temp.  Vel is the		*
 *  velocity of the boundary.						*
 *									*
 *  Original:	SEH	8/91   (Based on Arsenic.c by MEL)              *
 *									*
 ************************************************************************/
void Snboundary(
struct bound_str *bval)
{
    double f;
    double h,m;
    int sol;		/*the solution location of antimony*/
    int row0 = bval->loc[0][0];
    int row1 = bval->loc[1][1];
    int cp0  = bval->loc[0][1];
    int cp1  = bval->loc[1][0];
    int mat0 = bval->mat[0];
    int mat1 = bval->mat[1];
    float temp = bval->temp;

    sol = imptosol[iSn];

    /*calculate the transport terms*/
    m = Seg0(mat0,mat1) * exp ( - SegE(mat0, mat1) / (kb * temp) );
    h = sqrt( m ) * Trn0(mat0,mat1) * exp ( - TrnE(mat0, mat1) / (kb * temp) );

    /*material 1 side derivative of segregation terms*/
    f = h * bval->cpl;
    left_side(row0, sol, f);
    a[sol][sol][cp1] -= f;

    /*material 2 side derivative of segregation terms*/
    f = h * bval->cpl / m;
    left_side(row1, sol, f);
    a[sol][sol][cp0] -= f;

    /*right hand side of segregation terms*/
    f = bval->cpl * h * ( bval->conc[0] - bval->conc[1] / m );
    right_side(row0, sol, bval->rhs, -f);
    right_side(row1, sol, bval->rhs, f);

    /*compute the moving boundary flux - if required*/
    if ( (bval->dela[0] != 0.0) && (mat1 == SiO2) ) {
	a[sol][sol][row0]  -= bval->dela[0];
	a[sol][sol][cp1]   += bval->dela[0];
	f = bval->dela[0] * bval->conc[0];
	right_side( row0, sol, bval->rhs, f );
	right_side( row1, sol, bval->rhs, -f );
    }
    if ( (bval->dela[1] != 0.0) && (mat0 == SiO2) ) {
	a[sol][sol][row1]  -= bval->dela[1];
	a[sol][sol][cp0]   += bval->dela[1];
	f = bval->dela[1] * bval->conc[1];
	right_side( row1, sol, bval->rhs, f );
	right_side( row0, sol, bval->rhs, -f );
    }
    
    /*if either side is gas, disallow any update*/
    if ( mat0 == GAS ) {
	clear_row(row0, sol);
	left_side (row0, sol, 1.0);
    }
    if ( mat1 == GAS ) {
	clear_row(row1, sol);
	left_side (row1, sol, 1.0);
    }
}




/************************************************************************
 *									*
 *	Tin( par, param ) - This routine gets the 			*
 *  tin diffusion parameters from the cards and sets them up.		*
 *  They are then used by the rest of the program.			*
 *									*
 *  Original:	SEH	8/91   (Based on Arsenic.c by MEL)              *
 *									*
 ************************************************************************/
void tin(
char *par,
struct par_str* param)
{ 
    int mat;
    int mat2 = -1;
    register int i,j;
    int tmpfl, imp_flag, gro_flag;
    double t, c;

    /* get type */
    if ( Listed( "acceptor" ) ) {
	SET_FLAGS(iSn, ACTIVE_ACCEPTOR);
	SET_FLAGS(iSna, ACTIVE_ACCEPTOR);
    }
    if ( Listed( "donor" ) ) {
	CLEAR_FLAGS(iSn, ACTIVE_ACCEPTOR);
	CLEAR_FLAGS(iSna, ACTIVE_ACCEPTOR);
    }

    /*get the material number specified*/
    if ( get_bool( param, "silicon" ) )	mat = Si;
    if ( get_bool( param, "oxide" ) )	mat = SiO2;
    if ( get_bool( param, "oxynitride" ) )	mat = OxNi;
    if ( get_bool( param, "poly" ) )	mat = Poly;
    if ( get_bool( param, "nitride" ) )	mat = SiNi;
    if ( get_bool( param, "gas" ) )	mat = GAS;
    if ( get_bool( param, "gaas" ) )	mat = GaAs;

    /*fetch the values for each constant in this material*/
    i = 0; j = 1;
    imp_flag = get_bool( param, "implanted" );
    gro_flag = get_bool( param, "grown.in" );
    if ( gro_flag & !imp_flag ) j = 0;	/* parameters for grown.in only */
    if ( imp_flag & !gro_flag ) i = 1;	/* parameters for implanted only */

    tmpfl = GET_FLAGS(iSn);			/* save state */
    for (; i <= j ; i++) {
	if ( i == 0 )	CLEAR_FLAGS(iSn, IMPLANTED_IMP);
	else		SET_FLAGS(iSn, IMPLANTED_IMP);
	Fetch( Dix0(mat) , "Dix.0" );
	Fetch( DixE(mat) , "Dix.E" );
	Fetch( Dim0(mat) , "Dim.0" );
	Fetch( DimE(mat) , "Dim.E" );
	Fetch( Dimm0(mat) , "Dimm.0" );
	Fetch( DimmE(mat) , "Dimm.E" );
	Fetch( Fi(mat)   , "Fi" );
    }

    /* reset implanted source state */
    CLEAR_FLAGS(iSn, ALL_FLAGS);
    SET_FLAGS(iSn, tmpfl);

    /*deal with the solid solubility numbers*/
    if ( get_bool( param, "ss.clear") && is_specified( param, "ss.clear" )) {
	Snssmax = 0;
	Snsstemp = 0.0;
	Snsolid[0][0] = 0.0;
    }
    if ( is_specified( param, "ss.conc" ) && is_specified( param, "ss.temp" ) ) {
	t = get_float( param, "ss.temp" ) + 273.0;
	c = get_float( param, "ss.conc" );

	/*find the spot to store these at*/
	for(i = 0; (i < Snssmax) && (t > Snsolid[0][i]); i++);

	/*is this guy equal?*/
	if ( t == Snsolid[0][i] ) 
	    Snsolid[1][i] = c;
	else {
	    /*move everyone after up a spot*/
	    for(j = Snssmax-1; j >= i; j--) {
		Snsolid[0][j+1] = Snsolid[0][j];
		Snsolid[1][j+1] = Snsolid[1][j];
	    }
	    Snssmax++;

	    /*insert the new one*/
	    Snsolid[0][i] = t;
	    Snsolid[1][i] = c;
	}
	Snsstemp = 0.0;
    }

    /*now fetch any segregation data that comes our way*/
    if ( Listed( "/silicon" ) )	mat2 = Si;
    if ( Listed( "/oxide" ) )	mat2 = SiO2;
    if ( Listed( "/oxynitride" ) ) mat2 = OxNi;
    if ( Listed( "/poly" ) )	mat2 = Poly;
    if ( Listed( "/nitride" ) )	mat2 = SiNi;
    if ( Listed( "/gas" ) )	mat2 = GAS;
    if ( Listed( "/gaas" ) )	mat2 = GaAs;
    
    if (mat2 != -1) {
	if (is_specified(param, "Seg.0")) {
	    Seg0(mat2, mat) = get_float( param, "Seg.0" );
	    Seg0(mat, mat2) = 1.0 / Seg0(mat2, mat);
	}
	if (is_specified(param, "Seg.E")) {
	    SegE(mat2, mat) = get_float( param, "Seg.E" );
	    SegE(mat, mat2) = - SegE(mat2, mat);
	}
	if (is_specified(param, "Trn.0")) {
	    Trn0(mat, mat2) = get_float( param, "Trn.0" );
	    Trn0(mat2, mat) = Trn0(mat, mat2);
	}
	if (is_specified(param, "Trn.E")) {
	    TrnE(mat, mat2) = get_float( param, "Trn.E" );
	    TrnE(mat2, mat) = TrnE(mat, mat2);
	}
    }
}

