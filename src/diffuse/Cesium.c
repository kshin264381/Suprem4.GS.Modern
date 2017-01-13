/*   Cesium.c                Version 1.7     */
/*   Last Modification : 7/21/94 15:51:03        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
/*************************************************************************
 *									 *
 *     Copyright c 1987 The board of trustees of the Leland Stanford 	 *
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
#define D0(M)	impur[Cs].constant[M][1][IS_IMPLANTED(Cs)]
		/*exp constant with neutral V*/
#define DE(M)	impur[Cs].constant[M][2][IS_IMPLANTED(Cs)]

#define G0 4
#define GE 5

/*definitions of the segregation coefficients as a function of material*/
#define Seg0(M1, M2) 	impur[Cs].seg[SEG0][M1][M2]
#define SegE(M1, M2) 	impur[Cs].seg[SEGE][M1][M2]
#define Trn0(M1, M2) 	impur[Cs].seg[TRN0][M1][M2]
#define TrnE(M1, M2) 	impur[Cs].seg[TRNE][M1][M2]
#define g0(M1, M2) 	impur[Cs].seg[G0][M1][M2]
#define gE(M1, M2) 	impur[Cs].seg[GE][M1][M2]


/************************************************************************
 *									*
 *	This file contains definitions of the cesium routines.  It 	*
 *  includes the routine to calculate diffusion coefficients, the one	*
 *  to calculate any coupling terms between species, and the one to	*
 *  to calculate the boundary condition terms for the cesium.		*
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
 *	Csdiff_coeff(temp, noni, inter, vacan) - This routine returns	*
 *  the cesium diffusivity as a function of temperature, n/ni, 	*
 *  interstitial and vacancy concentrations.				*
 *									*
 *  Original:	MEL	1/85						*
 *  Revised:	MEL	3/86	(multiple materials)			*
 *									*
 ************************************************************************/
void Csdiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/)
{
    register int i;
    int mat;
    double dif[MAXMAT];
    double diff;

    /*first calculate the fixed stuff*/
    for(i = 0; i < MAXMAT; i++) {
	dif[i] = D0(i) * exp ( - DE(i) / (kb * temp) );
    }

    for(i = 0; i < nn; i++) {
	mat = nd[i]->mater;

	/*calculate the total diffusivity*/
	diff = dif[mat];

	idf[i] = diff;
	vdf[i] = 0.0;

	iprt[i] = vprt[i] = 0.0;
    }

}






/************************************************************************
 *									*
 *	Csboundary - This routine handles the computation of the 	*
 *  cesium boundary conditions at node i and solutions j and k.  The 	*
 *  time step is delta and the temperature is temp.  Vel is the 	*
 *  velocity of the boundary.						*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void Csboundary(
struct bound_str *bval)
{
    double f;
    double h,m,g;
    int sol;		/*the solution location of antimony*/
    int row0 = bval->loc[0][0];
    int row1 = bval->loc[1][1];
    int cp0  = bval->loc[0][1];
    int cp1  = bval->loc[1][0];
    int mat0 = bval->mat[0];
    int mat1 = bval->mat[1];
    float temp = bval->temp;

    sol = imptosol[Cs];

    /*calculate the transport terms*/
    m = Seg0(mat0,mat1) * exp ( - SegE(mat0, mat1) / (kb * temp) );
    h = sqrt( m ) * Trn0(mat0,mat1) * exp ( - TrnE(mat0, mat1) / (kb * temp) );
    g = g0(mat0,mat1) * exp ( - gE(mat0, mat1) / (kb * temp) );

    /*material 0 side derivative of segregation terms*/
    f = bval->cpl * (h - g * bval->conc[1]);
    left_side(row0, sol, f);
    a[sol][sol][cp1] -= f;

    /*material 1 side derivative of segregation terms*/
    f = bval->cpl * (h / m + g * bval->conc[0]);
    left_side(row1, sol, f);
    a[sol][sol][cp0] -= f;

    /*right hand side of segregation terms*/
    f = bval->cpl * ( h * ( bval->conc[0] - bval->conc[1] / m ) -
		      g * bval->conc[0] * bval->conc[1] );
    right_side(row0, sol, bval->rhs, -f);
    right_side(row1, sol, bval->rhs, f);

    /*right hand side of segregation terms*/
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
 *	Cesium( par, param ) - This routine gets the 			*
 *  Cesium diffusion parameters from the cards and sets them up.	*
 *  They are then used by the rest of the program.			*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void cesium(
char *par,
struct par_str* param)
{ 
    int i, j;
    int tmpfl, imp_flag, gro_flag;
    int mat;
    int mat2 = -1;

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

    tmpfl = GET_FLAGS(Cs);			/* save state */
    for (; i <= j ; i++) {
	if ( i == 0 )	CLEAR_FLAGS(Cs, IMPLANTED_IMP);
	else		SET_FLAGS(Cs, IMPLANTED_IMP);
	Fetch( D0(mat) , "D.0" );
	Fetch( DE(mat) , "D.E" );
    }

    /* reset implanted source state */
    CLEAR_FLAGS(Cs, ALL_FLAGS);
    SET_FLAGS(Cs, tmpfl);

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
	if (is_specified(param, "g.0")) {
	    g0(mat, mat2) = get_float( param, "g.0" );
	    g0(mat2, mat) = g0(mat, mat2);
	}
	if (is_specified(param, "g.E")) {
	    gE(mat, mat2) = get_float( param, "g.E" );
	    gE(mat2, mat) = gE(mat, mat2);
	}
    }
}
