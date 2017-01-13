/*   tri6.c                Version 1.6     */
/*   Last Modification : 7/23/94 19:04:09        */
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

#define X 0
#define Y 1
#define XX 0
#define YY 1
#define XY 2


#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "finel.h"

/*
void do_tri6_nodal_stress(
     double *xl,
     double *ul,
     double epsn[6][3],
     double sign[6][3],
     void (*coeff)(double *, int, double *, double *),
     int mat,
     double *xsj,
     int smooth)
*/
void do_tri6_nodal_stress(
     double *xl,
     double *ul,
     double *epsn,
     double *sign,
     void (*coeff)(double *, int, double *, double *),
     int mat,
     double *xsj,
     int smooth)

{
    static double locg[][2] = {{0.5,0.5},{0,0.5},{0.5,0}};
    double epsg[3][3], sigg[3][3];
    int j, l, c;

    /* Compute at each of the gauss points */
    for (l = 0; l < 3; l++) {
	tri6_stress( mat, coeff, xl, ul, locg[l], epsg[l], sigg[l], xsj);
    }

    /* Extrapolate to nodes */
    for (j = 0; j < 3; j++) {
	for (c = 0; c < 3; c++) {
	    *(epsn + 3 * (j+3) + c) = epsg[j][c];
	    *(sign + 3 * (j+3) + c) = sigg[j][c];
	    if (smooth) {
		*(epsn + 3*j + c) = epsg[(j+1)%3][c] + epsg[(j+2)%3][c] - epsg[j][c];
		*(sign + 3*j + c) = sigg[(j+1)%3][c] + sigg[(j+2)%3][c] - sigg[j][c];
	    }
	    else {
		*(epsn + 3*j + c) = (epsg[0][c] + epsg[1][c] + epsg[2][c])/3;
		*(sign + 3*j + c) = (sigg[0][c] + sigg[1][c] + sigg[2][c])/3;
	    }
	}
    }
}

/************************************************************************
 *									*
 *	FEshape - 6-noded triangle shape functions			*
 *									*
 ************************************************************************/
/*
void tri6_shape(
    double s, double t,
    double cord[6][2],
    double *xsj,	
    double shp[6][3])	
*/
void tri6_shape(
    double s, double t,
    double *cord,
    double *xsj,	
    double *shp)
{
    int i, j, k;
    double lam0, lam1, lam2, xs[2][2], sx[2][2], tmpx, tmpy;

    /* The linear shape functions...*/
    lam0 = 1-s-t;
    lam1 = s;
    lam2 = t;

    /* The parabolic shape functions...*/
    *(shp + 3*0 + 2) = lam0 * (2 * lam0 - 1);
    *(shp + 3*1 + 2) = lam1 * (2 * lam1 - 1);
    *(shp + 3*2 + 2) = lam2 * (2 * lam2 - 1);
    *(shp + 3*3 + 2) = 4 * lam1 * lam2;
    *(shp + 3*4 + 2) = 4 * lam2 * lam0;
    *(shp + 3*5 + 2) = 4 * lam0 * lam1;

    /* The derivatives...*/
    *(shp + 3*0 + 0) = -1 * (4 * lam0 - 1);
    *(shp + 3*0 + 1) = -1 * (4 * lam0 - 1);

    *(shp + 3*1 + 0) = 4 * lam1 - 1;
    *(shp + 3*1 + 1) = 0;

    *(shp + 3*2 + 0) = 0;
    *(shp + 3*2 + 1) = 4 * lam2 - 1;

    *(shp + 3*3 + 0) = 4 * t;
    *(shp + 3*3 + 1) = 4 * s;

    *(shp + 3*4 + 0) = -4 * t;
    *(shp + 3*4 + 1) =  4 * (1 - s - 2 * t);

    *(shp + 3*5 + 0) =  4 * (1 - 2 * s - t);
    *(shp + 3*5 + 1) = -4 * s;

    /*The jacobian and its inverse*/
    for (i=0; i < 2; i++) {
	for (j = 0; j < 2; j++) {
	    xs[i][j] = 0;
	    for (k = 0; k < 6; k++) {
		xs[i][j] += *(cord + 2*k + j) * *(shp + 3*k + i);
    }}}
    *xsj = xs[0][0] * xs[1][1] - xs[0][1] * xs[1][0];
    if (*xsj < 1e-30) {
	fprintf(stderr, "warning: triangle has non-positive jacobian");
    }

    sx[0][0] = xs[1][1]/ *xsj;
    sx[1][1] = xs[0][0]/ *xsj;
    sx[0][1] = -xs[0][1]/ *xsj;
    sx[1][0] = -xs[1][0]/ *xsj;

    /*Form global derivatives*/
    for (k=0; k < 6; k++) {
	tmpx = *(shp + 3*k + 0)*sx[0][0] + *(shp + 3*k + 1)*sx[0][1];
	tmpy = *(shp + 3*k + 0)*sx[1][0] + *(shp + 3*k + 1)*sx[1][1];
	*(shp + 3*k + 0) = tmpx;
	*(shp + 3*k + 1) = tmpy;
    }

    /*run for cover*/
}



/************************************************************************
 *									*
 *	FEstiff - return the stiffness matrix of a linear elastic	*
 *			6-noded triangle				*
 *									*
 ************************************************************************/
/*
void tri6_stiff(
		double stiff[12][12],
		double wrhs[6][2],	
		double xl[6][2],	
		double disp[6][2],	
		void (*coeff)(double *, int, double *, double *),		
		int mat)
*/
void tri6_stiff(
		double *stiff,
		double *wrhs,
		double *xl,
		double *disp,
		void (*coeff)(double *, int, double *, double *),		
		int mat)
{
    static double sg[]={0.5,0,0.5}, tg[]={0.5,0.5,0}, wg=0.166666666666667;
    double d11,d12,d33,db00,db01,db10,db11,db20,db21;
    double shp[6][3];
    double xsj, dv, eps[3], sig[3];
    double matco[100];
    int l, i, j;

    /*for each integration point compute contribution to stiffness*/
    for (l=0; l < 3; l++) {

	tri6_shape( sg[l], tg[l], xl, &xsj, &shp[0][0]);
	dv = wg*xsj;

	/* Current strains */
	eps[XX] = eps[YY] = eps[XY] = 0;
	for (j = 0; j < 6; j++) {
	    eps[XX] += shp[j][X] * *(disp + 2*j + X);
	    eps[YY] += shp[j][Y] * *(disp + 2*j + Y);
	    eps[XY] += shp[j][X] * *(disp + 2*j + Y) + shp[j][Y] * *(disp + 2*j +X);
	}

	/* Calculate the current material coeffs and stresses*/
	(*coeff) (matco, mat, eps, sig);
	
	d11 = matco[ 0]*dv;
	d12 = matco[ 1]*dv;
	d33 = matco[ 2]*dv;

	for (j = 0; j < 6; j++) {

	    /*compute rhs contribution*/
/*	    wrhs[ j][ X] -= dv * (sig[ XX]*shp[j][X] + sig[ XY]*shp[j][Y]);
	    wrhs[ j][ Y] -= dv * (sig[ XY]*shp[j][X] + sig[ YY]*shp[j][Y]);
*/

	    *(wrhs + 2 * j + X) -= dv * (sig[ XX]*shp[j][X] + sig[ XY]*shp[j][Y]);
	    *(wrhs + 2 * j + Y) -= dv * (sig[ XY]*shp[j][X] + sig[ YY]*shp[j][Y]);


	    /*for each j node compute db = d*b*/
	    db00 = d11*shp[j][0];
	    db01 = d12*shp[j][1];
	    db10 = d12*shp[j][0];
	    db11 = d11*shp[j][1];
	    db20 = d33*shp[j][1];
	    db21 = d33*shp[j][0];

	    /*for each i node compute s = bTdb*/
	    for (i=0; i <= j; i++) 
	      {
	       *(stiff + 12 * ( 2*i ) + 2*j) += shp[i][0]*db00 + shp[i][1]*db20;
	       *(stiff + 12 * ( 2*i ) + 2*j+1) += shp[i][0]*db01 + shp[i][1]*db21;
	       *(stiff + 12 * ( 2*i+1) + 2*j) += shp[i][0]*db20 + shp[i][1]*db10;
	       *(stiff + 12 * ( 2*i+1) + 2*j+1) += shp[i][0]*db21 + shp[i][1]*db11;

/* this was Conor's.  But to get C++ to work with this, I had to do that above
   -Zak.
            for (i=0; i <= j; i++) {
                stiff[ 2*i  ][ 2*j  ] += shp[i][0]*db00 + shp[i][1]*db20;
                stiff[ 2*i  ][ 2*j+1] += shp[i][0]*db01 + shp[i][1]*db21;
                stiff[ 2*i+1][ 2*j  ] += shp[i][0]*db20 + shp[i][1]*db10;
                stiff[ 2*i+1][ 2*j+1] += shp[i][0]*db21 + shp[i][1]*db11;
*/

	    }
	}
    }

    /*compute lower triangular part by symmetry*/
    for( j=1; j < 12; j++)
	for( i=0; i < j; i++)
	    *(stiff + 12 * j + i) = *(stiff + 12 * i + j);

/* this was Conor's line.  I had to change it to get C++ to work.
   stiff[j][i] = stiff[i][j];
*/

    return;
}





/************************************************************************
 *									*
 *	FEepsg()	-  pointwise stress, strain			*
 *									*
 ************************************************************************/
/*
void tri6_stress(
    int mat,
    int (*coeff)(double *, int, double *, double *),
    double xl[6][2],
    double ul[6][2],
    double *where,
    double eps[3],
    double sig[3],
    double *xsj)
*/
void tri6_stress(int mat,
		 void (*coeff)(double *, int, double *, double *),
		 double *xl,
		 double *ul,
		 double *where,
		 double *eps,
		 double *sig,
		 double *xsj)
{
    double shp[6][3]; int j;
    double matco[20];
    
    tri6_shape( where[0], where[1], xl, xsj, &shp[0][0]);

    /* Calculate strains */
    eps[ XX] = eps[ YY] = eps[ XY] = 0;
    for (j=0; j < 6; j++) {
	eps[ XX] += shp[j][X] * *(ul + 2*j + X);
	eps[ YY] += shp[j][Y] * *(ul + 2*j + Y);
	eps[ XY] += shp[j][X] * *(ul + 2*j + Y) + shp[j][Y] * *(ul + 2*j + X);
    }
    
    /* Call the strain to stress subroutine */
    (*coeff) ( matco, mat, eps, sig);
}

/*-----------------TRI6_NODAL_STRESS---------------------------------*
 * Compute the strains, stress at the corners by linear extrapolation.
 *--------------------------------------------------------------------*/
void tri6_Snodal_stress(
     double *xl,
     double *ul,
     double *epsn,
     double *sign,
     void (*coeff)(double *, int, double *, double *),
     int mat,
     double *xsj)
{
	do_tri6_nodal_stress( xl, ul, epsn, sign, coeff, mat, xsj, 1);
}

#ifdef DEBUG
#include <math.h>
void test_tri6()
{
    int i, j, c;
    double rr, ss, sum, sumx, sumy, xl[2], xr[2], derror;
    double xsj, shp[6][3], shp2[6][3];

    static double garbage[6][2] = { {0,0},{1,1},{-1,1}};
    for (i = 3; i < 6; i++) for (j = 0; j < 2; j++)
	garbage[i][j] = 0.5*(garbage[(i+1)%3][j] + garbage[(i+2)%3][j]);

    for (rr = 0; rr < 1; rr+= 0.2) {
	for (ss = 0; ss < 1; ss+= 0.2) {
	    if (rr + ss > 1) continue;
	    tri6_shape( rr, ss, garbage, &xsj, shp);

	    sum = sumx = sumy = 0;
	    for (i = 0; i < 6; i++) {
		sum += shp[i][2];
		sumx += shp[i][X];
		sumy += shp[i][Y];
	    }
	    sum -= 1.0;
	    if (fabs(sum) >= 1e-10) printf("[%g %g] sum %g\n",rr,ss,sum);
	    if (fabs(sumx) >= 1e-10) printf("[%g %g] sumx %g\n",rr,ss,sumx);
	    if (fabs(sumy) >= 1e-10) printf("[%g %g] sumy %g\n",rr,ss,sumy);

	    /* See if the derivatives really are */
	    tri6_shape( rr+1e-2, ss+1e-2, garbage, &xsj, shp2);
	    for (c = 0; c < 2; c++) {
		xl[c] = 0;
		xr[c] = 0;
		for (j = 0; j < 6; j++) xl[c] += shp[j][2]*garbage[j][c];
		for (j = 0; j < 6; j++) xr[c] += shp2[j][2]*garbage[j][c];
	    }
	    for (j = 0; j < 6; j++) {
		derror = shp2[j][2] - shp[j][2] 
			 - (xr[0]-xl[0])*(shp2[j][0] + shp[j][0])/2
			     - (xr[1]-xl[1])*(shp2[j][1] + shp[j][1])/2;

		if( derror > 1e-4 || derror < -1e-4)
		    printf("derivative error \n");
	    }
	}
    }
}

#endif
