/*   Oxidant.c                Version 1.4     */
/*   Last Modification : 7/21/94 22:12:51        */
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
#include "dbase.h"
#include "impurity.h"
#include "matrix.h"		/* where Odiff, Obc put things */
#include "material.h"
#include "diffuse.h"
#include "oxide.h"
#include "shell.h"

/*
 * Oxidant diffusion and reaction coefficients in SiO2 are derived
 * from the Deal-Grove coeffs, whose constants are stored in material.h
 * These numbers are mostly to allow people to diffuse oxidant in
 * say thin nitrides.
 */

/*definitions for impurity constants as a function of material*/
/*most useful when M==SiO2 and S={O2,H2O}*/
#define N1(S,M)	impur[S].constant[M][0][0] /*oxidant atoms/cc material*/
#undef H
#define H(S,M) impur[S].constant[M][1][0] /*Henry's law coeff*/

#define D0(S,M) impur[S].constant[M][2][0] /*pre ex constant of diffusivity*/
#define DE(S,M) impur[S].constant[M][3][0] /*energy constant of diffusivity*/

/*coefficients which are boundary - related. */
#define Seg0(S,M1,M2) 	impur[S].seg[0][M1][M2]
#define SegE(S,M1,M2) 	impur[S].seg[1][M1][M2]
#define Trn0(S,M1,M2) 	impur[S].seg[2][M1][M2]
#define TrnE(S,M1,M2) 	impur[S].seg[3][M1][M2]

extern double KsFermiDep();

/************************************************************************
 *									*
 *	This file contains definitions of the oxidant routines.  It 	*
 *  includes the routine to calculate diffusion coefficients, the one	*
 *  to calculate any coupling terms between species, and the one to	*
 *  to calculate the boundary condition terms for the oxidant.		*
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
 *	Odiff_coeff(temp, noni, inter, vacan) - This routine returns	*
 *  the oxidant diffusivity as a function of temperature, n/ni,		*
 *  interstitial and vacancy concentrations.				*
 *									*
 *  Two entries are provided to handle O2 and H2O cases.		*
 *									*
 *  Original:	CSR	4/86						*
 *									*
 ************************************************************************/

/* This is the real thing - it returns the diffusivity in one material for
   one species at one temperature.
   As a (misguided) performance hack, it calculates all materials upfront.*/

double Odiff_coeff(
    int m, int s,			/* material, species */
    float temp)			/* why sigh */
{
    static double Dix[MAXMAT];
    static double o_temp=0, o_press=0, o_s=(-1);
    int i;
    
    do_deal_grove( temp, s);

    if (o_temp != temp || o_press != pressure || o_s != s) {
	o_temp = temp;
	o_press = pressure;
	o_s = s;

	for (i = 0; i < MAXMAT; i++) {
	    /* Default arrhenius model */
	    Dix[ i] = D0( s, i) * exp( -DE (s, i) / (kb * temp) );

	    /* Special case for SiO2: D = B/2 * N1/C */
	    if (i == SiO2)
		Dix[ i] = B_DealGrove /2 * N1( s, i) / (H (s, i) * pressure);
	}
    }
    return( Dix[ m]);
}

/* Vector version to write diffusivity into each node cell */
void Odiff_vcoeff(
		    int s,		/* Which species we have */
		    float temp,
		    double **nuevo,
		    double **equil,
		    double *noni,
		    double *idf,
		    double *vdf,
		    double *iprt,
		    double *vprt
		    )
{
 int i;
 for(i = 0; i < nn; i++) 
   {
    idf[i] = Odiff_coeff( nd[i]->mater, s, temp );
    vdf[i] = iprt[i] = vprt[i] = 0.0;
   }
}

/* Interface to impurity table */
void O2diff_coeff(
		  float temp,
		  double **nuevo,
		  double **equil,
		  double *noni,
		  double *idf,
		  double *vdf,
		  double *iprt,
		  double *vprt
		  )
{ 
 Odiff_vcoeff (O2, temp, nuevo, equil, noni, idf, vdf, iprt, vprt); 
}

void H2Odiff_coeff(
		  float temp,
		  double **nuevo,
		  double **equil,
		  double *noni,
		  double *idf,
		  double *vdf,
		  double *iprt,
		  double *vprt
		  )
{ 
 Odiff_vcoeff (H2O, temp, nuevo, equil, noni, idf, vdf, iprt, vprt);
}



/************************************************************************
 *									*
 *	Oboundary - This routine handles the computation of the 	*
 *  Oxidant boundary conditions at node i and solutions j and k.  The 	*
 *  time step is delta and the temperature is temp.  Vel is the 	*
 *  velocity of the boundary.						*
 *  This comment is about three years out of date.			*
 *									*
 *  Original:	CSR	4/86						*
 *									*
 ************************************************************************/
void Oboundary (
    int s,			/*which species we have*/
    struct bound_str *bval)
{
    double f;
    double h,m;
    int sol;		/*the solution location of oxidant*/
    int O2sol, Nsol;
    int mat0, mat1, nx0;
    int row0, row1, cp0, cp1;
    double dln[2];

    sol = imptosol[s];

    /*avoid a lot of "if" statements by making mat0 oxide if either is*/
    if (bval->mat[1] == SiO2) {
	O2sol = 1;
	Nsol = 0;
    }
    else {
	O2sol = 0;
	Nsol = 1;
    }

    mat0 = bval->mat[O2sol];
    mat1 = bval->mat[Nsol];
    row0 = bval->loc[O2sol][O2sol];
    row1 = bval->loc[Nsol][Nsol];
    cp0  = bval->loc[O2sol][Nsol];
    cp1  = bval->loc[Nsol][O2sol];
    nx0  = bval->nx[O2sol];

    /*calculate the transport fluxes*/
    h = Trn0( s, mat0, mat1) * exp( -TrnE( s, mat0, mat1) / (kb * bval->temp));
    m = Seg0( s, mat0, mat1) * exp( -SegE( s, mat0, mat1) / (kb * bval->temp));

    /*special case when mat1 is silicon and mat0 is oxide*/
    if (mat1 == Si && mat0 == SiO2) {
	
	/*to compute the reaction rate, need the orientation.*/
	(void)dlocal_normal( nx0, Si, dln);
	h = Orr(bval->temp, s, dln, noni[ bval->nx[ Nsol]]);

        /*material 1 side derivitive*/
        f = h * bval->cpl;
        left_side( row0, sol, f );

	/*right side terms*/
	f = bval->cpl * h * bval->conc[O2sol] ;
	right_side(row0, sol, bval->rhs, -f);
    }
    else {

	if (h == 0.0) {
	    clear_row(row1, sol);
	    left_side (row1, sol, 1.0);
	    right_side(row1, sol, bval->rhs, 0.0);
	}
	else {
	    /*material 1 side derivative*/
	    f = h * bval->cpl;
	    left_side(row0, sol, f);
	    a[sol][sol][cp1] -= f;

	    /*material 2 side term*/
	    f = h * bval->cpl / m;
	    left_side(row1, sol, f);
	    a[sol][sol][cp0] -= f;

	    /*right hand side terms*/
	    f = bval->cpl * h * ( bval->conc[O2sol] - bval->conc[Nsol] / m );
	    right_side(row0, sol, bval->rhs, -f);
	    right_side(row1, sol, bval->rhs, f);
	}
	
    }
    /*disallow update to gas or silicon side*/
    if (mat1 == Si || mat1 == GAS) {
	clear_row(row1, sol);
	left_side (row1, sol, 1.0);
	right_side(row1, sol, bval->rhs, 0.0);
    }

}

void O2boundary(struct bound_str *bval) { Oboundary (O2, bval); }

void H2Oboundary(struct bound_str *bval) { Oboundary (H2O, bval); }

/*-----------------OSS--------------------------------------------------
 * The oxygen solid solubility in various materials.
 * Original: CSR 4/86
 *----------------------------------------------------------------------*/
float Oss (
    int s,		/* Which species it is */
    int mat)		/* What material we're in */
{
    /* Henry's law rules OK */
    return ( H( s, mat) * pressure);
}

/*-----------------Orr--------------------------------------------------
 * The reaction rate constant, in its purest form.
 * Original: CSR 8/87
 *----------------------------------------------------------------------*/
float Orr (float temp, int s, double *normal, double noni)
{
    float val; double XtalDir[3];
    
    /* B, B/A at this temperature */
    do_deal_grove (temp, s);

    /* Convert to ks */
    val = B_DealGrove/A_DealGrove * N1(s, SiO2) / (H (s, SiO2) * pressure);

    /* Add in orientation dependence */
    SupToXtal( normal, XtalDir);
    val *= DoOriDep( XtalDir, oridep);

    /* Add in fermi-level enhancement */
    val *= KsFermiDep( noni, temp);
    
    return( val );
}


/*-----------------Ovel-------------------------------------------------
 * Compute dx/dt from oxidant concentration
 * Original: CSR 4/86
 *----------------------------------------------------------------------*/
float Ovel (
    float temp,			/* Temperature */
    int s,			/* Which species */
    float conc,			/* Oxidant concentration */
    double *n,			/* XY normal */
    double noni)		/* electron excess */
{
    float v, l, orr;

    /* ks C / N1 */
    orr = Orr( temp, s, n, noni);
    v   = orr * conc / N1( s, SiO2);

    if( s == O2) {
	/* The thin oxide correction: figure an effective length
	 * and plug it into the thin oxide correction term.
	 * Note conc is lagged one timestep.  Oh, well.
	 */
	l = LenEff( conc, orr);
	v += ThinOxideCorr( l, n);
    }
    return (v);
}



/*-----------------LenEff-----------------------------------------------
 * Calculate the effective thickness at a point.
 * Algorithm is to look at the concentration, compare it to C* and figure
 * from the reaction rate coefficients what thickness of oxide would
 * have caused that concentration.
 * Hassle: B/A can depend on position so it has to be passed in.
 *----------------------------------------------------------------------*/
float LenEff(
    float conc, float ks)		/* ks turns out more convenient than B/A */
{
    /* -D(C-C*) = kC .:.  X=D/ks(C* / C-1) */
    float D, Cstar;
    
    if( ks <= 0 || conc <= 0) return(MAXFLOAT);
    
    Cstar =  H (gas_type, SiO2) * pressure;
    D = B_DealGrove /2 * N1( gas_type, SiO2) / Cstar;
    
    return( D/ks * (Cstar / conc - 1));
}
    


/*-----------------KsFermiDep-------------------------------------------
 * Fermi-level dependent reaction rate.
 * From Plummer's class notes, based on:
 *
 *   C.P. Ho and J.D. Plummer, " Si/SiO2 Interface Oxidation Kinetics:
 *   A Physical Model for the Influence of High Substrate Doping Levels.
 *   II. Comparison with Experiment and Discussion," J. Electrochem.
 *   Soc, 126 1523-1530 (Sept 1979)
 *
 *----------------------------------------------------------------------*/
#define mabs(a) (((a)>=0)?(a):(-(a)))

double KsFermiDep(double noni, double temp)
{
    double Vratio, Kratio;
    static double oTemp, iVp, iVpp, iVn, iVnn, Kco;

    /* If the diffusion code hasn't calculated the Fermi level yet, forget it*/
    if( noni == 0) return(1.0);
    if (!BAf_dep) return(1.0);

    if( mabs(temp-oTemp) > 0.01) {
	/* Calculate intrinsic vacancy concentrations at this temperature */
	oTemp = temp;
	iVp  = BAf_ebkr* exp( -BAf_Vp  / (KB*temp));
	iVpp = BAf_ebk * exp( -BAf_Vpp / (KB*temp));
	iVn  = BAf_ebkr* exp( -BAf_Vn  / (KB*temp));
	iVnn = BAf_ebk * exp( -BAf_Vnn / (KB*temp));
	
	/* Temperature dependent rate coeff */
	Kco  = BAf_Kco0 * exp( -BAf_Kcoe  / (KB*temp));

    }
    
    /* Fermi level dependent concentration */
    Vratio = (1 + iVp/noni + iVpp/(noni*noni) + iVn*noni + iVnn*noni*noni)/
	     (1 + iVp      + iVpp             + iVn      + iVnn);

    Kratio = 1 + Kco * (Vratio - 1);

    return( Kratio);
}

#include "FEgeom.h"		/* for maxdof... */
#include "FEmath.h"		/* for lambda */

/*
 * Turn on stress dependences one by one or all together?
 */
static int byhalves=1;
static int xjacob=1;
/*-----------------FE_oxbulk--------------------------------------------
 * The bulk coefficients for the oxide finite element
 * Original: CSR 7/87
 *----------------------------------------------------------------------*/
#define X 0
#define Y 1
#define XX 0
#define YY 1
#define XY 2
void FE_oxbulk(
     double *matco,
     int mat,
     double *eps,
     double *sig)
{
    double mu, nu, Eyoung, tau, sigc, exx=eps[XX], eyy=eps[YY], exy=eps[XY];
    double s0xx, s0yy, s0xy, s0dp;
	double f, dfda, d0, d1, d2, dfdx, dfdy, dfdt;
	double de00, de01, de02, de10, de11, de12, de20, de21, de22;
        double dfds, sigd;
    int do_stress;
    extern float proc_temp;
    double clam, dlam;
    int nmu;

    if( mat != SiO2) nmu = mat;
    else if( gas_type != H2O) nmu = SiO2;
    else nmu = MAXMAT;

    mu = mu0[ nmu] * exp( mue[ nmu] / (KB * proc_temp));

    /*Calculate the corresponding Young's modulus,
      *using the shear modulus == viscosity analogy*/
    nu = nux[mat];
    Eyoung = mu * 2 * (1 + nu);

    /*Put in plane strain coefficients into the first three positions*/
    plane_strain( Eyoung, nu, matco);
    sig[XX] = s0xx = matco[0] * exx + matco[1] * eyy;
    sig[YY] = s0yy = matco[1] * exx + matco[0] * eyy;
    sig[XY] = s0xy = matco[2] * exy;
	
    do_stress = stress_dep && (mat == SiO2);
    if (do_stress) {
	if( byhalves) {
	    clam = lambda/0.25;
	    if (clam > 1) clam = 1;
	} else {
	    clam = lambda;
	}
	/* The critical stress at this temperature */
	sigc = (clam*Vc!=0)? 2 * 8.62e-5 * 1.6e-12 * proc_temp / (clam*Vc) : MAXFLOAT;

	/* let's be quite certain about it */
	s0dp  = s0xx - s0yy;
	tau = sqrt( 0.25*s0dp*s0dp + s0xy*s0xy);
	if (tau/sigc < 1e-6) do_stress = 0;
    }
	
    if (!do_stress) {
	/*tangential stiffness matrix*/
	matco[10] = matco[14] = matco[0];
	matco[11] = matco[13] = matco[1];
	matco[12] = matco[15] = matco[16] = matco[17] = 0;
	matco[18] = matco[2];
    }
    else {
	/*Just watch me now!*/
	d0 = matco[0];
	d1 = matco[1];
	d2 = matco[2];

	synch2( tau/sigc, &f, &dfda);
	sig[XX] = s0xx * f;
	sig[YY] = s0yy * f;
	sig[XY] = s0xy * f;

	if (!noderiv) {

	dfdx = dfda/sigc *  0.25*s0dp/tau * (d0-d1);
	dfdy = dfda/sigc * -0.25*s0dp/tau * (d0-d1);
	dfdt = dfda/sigc *       s0xy/tau * d2;

	/* dsig/deps = D0 ( fI + Df.eps ) */
	de00 = f + dfdx*exx; de01 =     dfdy*exx; de02 =     dfdt*exx;
	de10 =     dfdx*eyy; de11 = f + dfdy*eyy; de12 =     dfdt*eyy;
	de20 =     dfdx*exy; de21 =     dfdy*exy; de22 = f + dfdt*exy;
	matco[10] = d0*de00 + d1*de10;
	matco[11] = d0*de01 + d1*de11;
	matco[12] = d0*de02 + d1*de12;
	matco[13] = d1*de00 + d0*de10;
	matco[14] = d1*de01 + d0*de11;
	matco[15] = d1*de02 + d0*de12;
	matco[16] = d2*de20;
	matco[17] = d2*de21;
	matco[18] = d2*de22;
	matco[0] *= f;
	matco[1] *= f;
	matco[2] *= f;
        }
    }

    /* The oxidant diffusivity from B */
    matco[ 5] = Odiff_coeff( mat, gas_type, proc_temp );
    
    /* Stress dependent reduction */
    if (stress_dep) {
	if (byhalves) {
	    if (lambda < 0.5) dlam = 0;
	    else dlam = (lambda - 0.5)/0.5;
	} else {
	    dlam = lambda;
	}
	if (dlam*Vd != 0) {
	    sigd = 8.62e-5 * 1.6e-12 * proc_temp / (dlam*Vd);
	    exp_l( 0.5 * (sig[XX] + sig[YY])/sigd, &f, &dfda, dlam);
	    if( f > Dlim) {f = Dlim; dfda=0;}
	    dfds = 0.5 * dfda/sigd;
	}
	else {
	    f = 1.0;
	    dfds = 0.0;
	}
	matco[20] = f;
	if (!noderiv) {
	    if (xjacob) {
		matco[21+XX] = dfds*(matco[10]+matco[13]);
		matco[21+YY] = dfds*(matco[11]+matco[14]);
		matco[21+XY] = dfds*(matco[12]+matco[15]);
	    } else {
		matco[21+XX] = 0;
		matco[21+YY] = 0;
		matco[21+XY] = 0;
	    }
	}
    }
    else {
	matco[20] = 1.0;
	matco[21+XX] = matco[21+YY] = matco[21+XY] = 0;
    }

}


/*-----------------KSUBS------------------------------------------------
 * The surface reaction rate coefficient.
 * Original: CSR 7/87
 *----------------------------------------------------------------------*/
void ksubs(
    double *rval,		/* List of return values */
    double *sig, double *n,		/* Stress tensor, local normal */
    double *extra)		/* Info stored by arrangement with FEconvert */
{
    extern float proc_temp;
    double sigr, sign, arg, f1, dfda, dfds1, f2, dfds2, sigt, sigh;
    double rlam, noni=extra[0], conc=extra[1], rho;

    rval[3] = Orr( proc_temp, gas_type, n, noni);
    rho = (1 - alpha[SiO2][Si])/alpha[SiO2][Si];
    rval[4] =  rho / N1( gas_type, SiO2);
    if( gas_type == O2)
	rval[10] = rho * ThinOxideCorr( LenEff( (float) conc, (float) rval[3]), n);
    else
	rval[10] = 0.0;
    
    /* Stress dependent reduction */
    if (stress_dep) {
	if( byhalves) {
	    if (lambda < 0.25) rlam = 0;
	    else if (lambda > 0.5)  rlam = 1;
	    else rlam = (lambda - 0.25) / 0.25;
	} else {
	    rlam = lambda;
	}
	if (rlam*Vr != 0) {
	    sigr = 8.62e-5 * 1.6e-12 * proc_temp / (rlam*Vr);
	    sign = sig[XX]*n[X]*n[X] + sig[YY]*n[Y]*n[Y] + 2*sig[XY]*n[X]*n[Y];

	    arg = sign/sigr;
	    exp_l( arg, &f1, &dfda, rlam);
	    if( f1 > 1) {f1 = 1; dfda = 0;}

	    dfds1 = dfda/sigr;

	}
	else {
	    f1 = 1.0;
	    dfds1 = 0;
	}
	if (rlam * Vt != 0) {
	    sigt = 8.62e-5 * 1.6e-12 * proc_temp / (rlam*Vt);
	    sigh = sig[XX]*n[Y]*n[Y] + sig[YY]*n[X]*n[X] - 2*sig[XY]*n[X]*n[Y];
	    
	    arg = sigh/sigt;
	    exp_l( arg, &f2, &dfda, rlam);
	    if( f2 > 1) {f2 = 1; dfda = 0;}

	    dfds2 = dfda/sigt;

	}
	else {
	    f2 = 1.0;
	    dfds2 = 0;
	}
	if( xjacob) {
	    rval[XX] = rval[3] * (f2 * dfds1 * n[X]*n[X] + f1 * dfds2 * n[Y]*n[Y]);
	    rval[YY] = rval[3] * (f2 * dfds1 * n[Y]*n[Y] + f1 * dfds2 * n[X]*n[X]);
	    rval[XY] = rval[3] * (f2 * dfds1 - f1 * dfds2) * 2*n[X]*n[Y];
	} else {
	    rval[XX] = rval[YY] = rval[XY] = 0;
	}
	rval[5] = f1*f2;
    }
    else {
	rval[XX] = rval[YY] = rval[XY] = 0;
	rval[5] = 1.0;
    }
}
    

/*-----------------ThermSig---------------------------------------------
 * Evaluate the thermal stresses.
 *----------------------------------------------------------------------*/
static double istrain[MAXMAT][3];
static double istress[MAXMAT][3];

void ThermSig(float temp1, float temp2)
{
    int r, m, l;
    float Ta, dT, exp, temp, alph;
    static float wg3[3]={5.0/9.0, 8.0/9.0, 5.0/9.0}, lg3[3] = {-0.7745966692, 0, 0.7745966692};
    int used[MAXMAT];
    
    for (m = 0; m < MAXMAT; m++) {
	istrain[m][XX] = istrain[m][YY] = istrain[m][XY] = 0;
	used[m] = 0;
    }
    for (r = 0; r < nreg; r++)
	used[ mat_reg(r)] = 1;

    Ta = 0.5*(temp1+temp2);
    dT = 0.5*(temp2-temp1);
    if (dT > -1e-3 && dT < 1e-3) return;

    /* For each material, integrate the lcte from temp1 to temp2 */
    /* Use simple 3-pt Gaussian integration */
    for (m = 0; m < MAXMAT; m++) {
	if (!used[m]) continue;
	if (!atherm[m]) continue;
	exp = 0;
	for (l = 0; l < 3; l++) {
	    temp = Ta + lg3[l]*dT;
	    fmacro("T", "%12e", temp);
	    alph = string_to_real( atherm[ m], -1.0);
	    umacro("T");
	    if (alph < 0) return;
	    exp += wg3[l] * alph;
	}
	exp *= dT;
	istrain[ m][XX] = istrain[m][YY] = exp;
    }
}

/*-----------------AddIntSig--------------------------------------------
 * Add nitride intrinsic stress, assuming it is all Sxx.
 *----------------------------------------------------------------------*/
void AddIntSig()
{
    int m;
    for (m = 0; m < MAXMAT; m++)
	istress[m][XX] = IntStress[m];
}
    
void plane_strain(
    double E, double v, double *matco)
{
    matco[0] = E * (1-v) / ((1+v)*(1-2*v));
    matco[1] = E *  v    / ((1+v)*(1-2*v));
    matco[2] = E         / ((1+v)*  2    );
}



/*-----------------SUP4_ECOEFF------------------------------------------
 * Coefficients for the elasticity solver.
 * Original: CSR 7/87
 *----------------------------------------------------------------------*/
void sup4_ecoeff(
     double *matco,
     int mat,
     double *eps, double *sig)
{
    int i; double feps[3];
    for (i = 10; i < 20; i++) matco[i] = 0;

    feps[XX] = eps[XX] + istrain[ mat][XX];
    feps[YY] = eps[YY] + istrain[ mat][YY];
    feps[XY] = eps[XY] + istrain[ mat][XY];
    
    plane_strain( E[mat], pr[ mat], matco);
    
    sig[XX] = matco[0] * feps[ XX] + matco[1] * feps[ YY];
    sig[YY] = matco[1] * feps[ XX] + matco[0] * feps[ YY];
    sig[XY] = matco[2] * feps[ XY];
    
    sig[XX] += istress[ mat][XX];
    sig[XY] += istress[ mat][XY];
    sig[YY] += istress[ mat][YY];
    return;
}

void synch2(
     double arg, double *val, double *deriv)
{
    double s;
    if (arg > -1e-6 && arg < 1e-6) {
	*val = 1; *deriv = 0;
    }
    else {
	s = asinh( arg);
	*val = s/arg;
	*deriv = (1/cosh(s) - *val)/arg;
    }
}

void exp_l(
    double arg, double *f, double *dfda, double lambda)
{	
    double argLim, eargl;
    argLim = 4.6;
    eargl = exp( argLim);
    
    if (arg < -argLim) {
	*f    = -1/(eargl*(arg + 1));
	*dfda =  1/(eargl*(arg + 1)*(arg + 1));
    }
    else {
	*f = exp(arg);
	*dfda = *f;
    }
}

#ifdef DEBUG
void test_bulk(
     double ex, double ey, double ez, double dx, double dy, double dz)
{
    double matco[30], matco1[30], sig[3], eps[3], sig1[3], a, b, c, det, i0, i1, i2; 
    nux[ SiO2] = 0.499; mu0[ SiO2] = 2413.9; mue[ SiO2] = 2.525; proc_temp=1273; Vc=200e-24;
    stress_dep = 1;


    eps[XX] = ex; eps[YY] = ey; eps[XY] = ez;
    FE_oxbulk( matco, SiO2, eps, sig);

    eps[XX] += dx; eps[YY] += dy; eps[XY] += dz;
    FE_oxbulk( matco1, SiO2, eps, sig1);

    
    printf("%g %g\n", sig1[XX]-sig[XX], matco[10]*dx + matco[11]*dy + matco[12]*dz);
    printf("%g %g\n", sig1[YY]-sig[YY], matco[13]*dx + matco[14]*dy + matco[15]*dz);
    printf("%g %g\n", sig1[XY]-sig[XY], matco[16]*dx + matco[17]*dy + matco[18]*dz);
    printf("%g %g\n", matco1[20]-matco[20],
	   matco[21+XX]*dx + matco[21+YY]*dy + matco[21+XY]*dz);
}
#endif

#ifndef HAVE_ASINH
double asinh(
     double arg)
{
    double s, delta;

    /* Initial guess */
    if (arg > 2) s = log(2*arg);
    else if (arg < -2) s = -log(-2*arg);
    else s = arg;

    /* Newton loop */
    do {
	delta = (arg - sinh(s))/cosh(s);
	s += delta;
    }
    while (delta > 1e-12 || delta < -1e-12);
    return(s);
}
#endif
