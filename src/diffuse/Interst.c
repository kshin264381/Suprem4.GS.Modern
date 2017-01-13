/*   Interst.c                Version 1.25     */
/*   Last Modification : 11/18/94 17:00:38        */
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
#include <string.h>
#include <strings.h>

#include "global.h"
#include "constant.h"
#include "geom.h"
#include "impurity.h"
#include "defect.h"
#include "material.h"
#include "diffuse.h"
#include "matrix.h"
#include "misc.h"
#include "plot.h"
#include "shell.h"
#include "oxide.h"
#include "dislocation.h"

/************************************************************************
 *									*
 *	This file contains definitions of the Interstitial routines. It *
 *  includes the routine to calculate diffusion coefficients, the one	*
 *  to calculate any coupling terms between species, and the one to	*
 *  to calculate the boundary condition terms for the Intersitial.	*
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
 *	Idiff_coeff(temp, noni, inter, vacan) - This routine calculates	*
 *  the interstitial diffusivity given the temperature, n/ni, and	*
 *  local defect concentrations.					*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void Idiff_coeff(
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
    double diff;

    for(i = 0; i < nn; i++) {
	mat = nd[i]->mater;

	/*calculate the total diffusivity*/
	diff = Di(I,mat);

	idf[i] = diff;
	vdf[i] = 0.0;

	iprt[i] = 0.0;
	vprt[i] = 0.0;
    }
}




/************************************************************************
 *									*
 *	Icoupling( ) - This routine computes the recombination of 	*
 *  interstitials with the other species.				*
 *									*
 *  Original:	MEL	2/85						*
 *									*
 ************************************************************************/
void Icoupling(
float temp,
double *area,		/*the nodal areas*/
double **znew,		/*all the concentrations*/
double **equil,		/*the equilibrium concentration*/
double **dequ,		/*the derivitive of equilibrium concentration*/
double **rhs		/*the current right hand side*/)
{
    register int is = imptosol[I];
    register int vs = imptosol[V];
    register int ps = imptosol[Psi];
    register int i, mat;
    double cpl, t1, t2;

    /*if we are solving for vacancies*/
    if ( (vs != -1)  ) {
	
	/*for each node, compute the recombination*/
	for(i = 0; i < nn; i++) {

	    /*get the material*/
	    mat = nd[i]->mater;

	    /*compute the coupling terms*/
	    cpl = Kr(I,mat) * area[i];
    
	    /*calculate left hand side terms*/
	    a[is][is][i] += cpl * znew[vs][i];
	    a[is][vs][i] += cpl * znew[is][i];
	    a[is][ps][i] -= cpl * (equil[is][i]*dequ[vs][i] + equil[vs][i]*dequ[is][i]);

	    /*calculate right hand side terms at new point*/
	    cpl *= equil[is][i] * equil[vs][i];
	    t1 = znew[is][i] / equil[is][i];
	    t2 = znew[vs][i] / equil[vs][i];
	    rhs[is][i] -= cpl * (t1 * t2 - 1.0);
	}
    }
}






/************************************************************************
 *									*
 *	Iboundary - This routine handles the computation of Interstitial*
 *  boundary conditions at node i and solutions j and k.  The time step	*
 *  is delta and the temperature is temp.  Vel is the velocity of the	*
 *  boundary.								*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void Iboundary(
struct bound_str *bval)
{
    register int Sisol;
    int sol;
    double tmp;
    double vloc;
    float vmax;
    double normal[2];
    int Osp = ((imptosol[H2O] == -1)?(O2):(H2O));
    double vpow, tpow;
    double K;
    int mat0 = bval->mat[0];
    int mat1 = bval->mat[1];
    int row;
    int imp = I;
    int ps = imptosol[Psi];
    double h,m,f;
    int row0 = bval->loc[0][0];
    int row1 = bval->loc[1][1];
    int cp0  = bval->loc[0][1];
    int cp1  = bval->loc[1][0];


    normal[0] = 0.0; normal[1] = -1.0;

    /*the surface condition is a mix of injection and recombination*/
    sol = imptosol[ imp ];

    /*make sure the mat1 stuff is silicon - ish */
    if ( (mat1 == Si) || (mat1 == GaAs)
			|| ( (mat0 != Si) && (mat1 == Poly) ) ) {
	Sisol = 1;
	row = bval->loc[1][1];
    }
    else {
	Sisol = 0;
	row = bval->loc[0][0];
    }

    /*compute the magnitude of the local velocity*/
    vloc = sqrt( bval->vel[0] * bval->vel[0] + bval->vel[1] * bval->vel[1] );

    /*compute the initial velocity of growth*/
    if ( imptosol[XVEL] != -1 )
	vmax = Ovel( bval->temp, Osp, Oss( Osp, SiO2 ), normal, 1.0 );
    else
	vmax = 1.0;

    /*do the surface recombination terms*/
    if ( model(I,mat0, mat1) & RECOM ) {
	/*get the velocity power*/
	if ( vloc != 0.0 ) 
	    vpow = pow( vloc / vmax, Kpow(I,mat0,mat1) );
	else
	    vpow = 0.0;

	/*if we have a formula...*/
	if ( recstr(I,mat0,mat1) != NULL ) 
	    K = form_eval( recstr(I,mat0,mat1), total+bval->delta, bval->cord );
	else 
	    /*compute the surface recombination velocity*/
	    K = Ksurf(I,mat0,mat1) * ( Krat(I,mat0,mat1) * vpow + 1.0 );

	/*calculate the surface recombination term*/
	tmp = bval->cpl * K;
	left_side(row, sol, tmp);
	tmp = bval->cpl * K * dequ[sol][row];
	a[sol][ps][row] -= tmp;

	/*right hand side term at new time step*/
	tmp = bval->cpl * K * ( bval->conc[Sisol] - bval->eq[Sisol] );
	right_side(row, sol, bval->rhs, -tmp);
    }

    /*growth model for injection*/
    if ( model(I,mat0, mat1) & GROWTH ) {
	/*get the velocity power*/
	if ( vloc != 0.0 )
	    vpow = pow( vloc / vmax, Gpow(I,mat0,mat1) );
	else
	    vpow = 0.0;

	/*compute the injection term*/
	tmp = - bval->dela[Sisol] * vmole(I,mat0,mat1) * theta(I,mat0,mat1) * vpow;
	right_side(row, sol, bval->rhs, tmp);
    }

    /*time dependent model for injection*/
    if ( model(I,mat0, mat1) & TIME ) {
	if ( injstr(I,mat0,mat1) != NULL )
	    tpow = form_eval( injstr(I,mat0,mat1), total+bval->delta, bval->cord );
	else
	    tpow = A(I,mat0,mat1) * 
		   pow( t0(I,mat0,mat1)+total+bval->delta, Tpow(I,mat0,mat1));
	tmp = bval->cpl * tpow;
	right_side(row, sol, bval->rhs, tmp);
    }

    if ( model(I,mat0, mat1) & SEGRE ) {
	m = Seg(I,mat0,mat1);
	h = sqrt( m ) * Trn(I,mat0,mat1);

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
    }
}





/************************************************************************
 *									*
 *	time_val( ) - This routine computes the time derivative of the	*
 *  specified impurity.							*
 *									*
 *  Original:	MEL	8/88 (rewrite of two older routines)		*
 *									*
 ************************************************************************/
void Itime_val(
int is,
double *rhsnm,
struct call_str *cs)
{
    register int i, j, s, si;
    int dop[MAXIMP], ndop, mat;
    int ip = which(I);
    double cpl, tot, tmp;
    double gam, igam;
    double t2, t3;
    double dtot, gprt[MAXCHG];
    double n;
    double es, kt, bt, at, *ot;
    double expa, dnda, dndb, jac;
    register int ts = imptosol[T];
    register int ps = imptosol[Psi];

    double dtot1, dtot2;
    register int vs = imptosol[V];
    double kti, ktv, den;

    if ( IS_PSEUDO(I) ) {
	for(i = 0; i < nn; i++) rhs[is][i] = newb[is][i];
	return;
    }
	
    /*compute the number of dopants*/
    for( ndop = j = 0; j < n_imp; j++) {
	switch( soltoimp[j] ) {
	case As :
	case B  :
	case Sb :
	case P  :
	case iBe :
	case iMg :
	case iSe :
	case iSi :
	case iSn :
	case iGe :
	case iZn :
	case iC  :
	case iG  :
	    dop[ndop++] = j;
	}
    }

    /*precompute the time coupling terms*/
    if ( cs->type == TR ) {
	cpl = 2.0 / cs->new_del;
	if ( ts != -1 ) ot = cs->old[ts];
    }
    else if ( cs->type == BDF ) {
	/* wieghting terms */
	gam = cs->old_del / ( cs->new_del + cs->old_del );
	igam = 1.0 - gam;
	cpl = (2.0 - gam) / cs->new_del;
	t2 = (1.0 / gam) / cs->new_del;
	t3 = (igam * igam / gam) / cs->new_del;
	if ( ts != -1 ) ot = cs->mid[ts];
    }

    /*since we can't get it vectorized anyway, one big outer loop*/
    for(i = 0; i < nn; i++) {
	cs->newa[is][i] = cs->nuevo[is][i];

	dtot1 = 1.0;
	dtot2 = 0.0;
/*	dtot = 1.0; */

	mat = nd[i]->mater;
	n = noni[i];

	if ( methdata.defmodel == FULLCPL ) {
	    /*compute the total partition value*/

/*
	    tot = Dfrac[ip][mat][CN]
		+ ((Dfrac[ip][mat][CTM] * n + Dfrac[ip][mat][CDM]) * n
			+ Dfrac[ip][mat][CM]) * n
		+ ((Dfrac[ip][mat][CTP] / n + Dfrac[ip][mat][CDP]) / n
			+ Dfrac[ip][mat][CP]) / n ;
*/
	    /*compute the partition functions*/
/*	    gprt[CN]  = Dfrac[ip][mat][CN] / tot;
	    gprt[CM]  = n * Dfrac[ip][mat][CM] / tot;
	    gprt[CDM] = n * n * Dfrac[ip][mat][CDM] / tot;
	    gprt[CTM] = n * n * n * Dfrac[ip][mat][CTM] / tot;
	    gprt[CP]  = Dfrac[ip][mat][CP] / (tot * n);
	    gprt[CDP] = Dfrac[ip][mat][CDP] / (tot * n * n);
	    gprt[CTP] = Dfrac[ip][mat][CTP] / (tot * n * n * n);
*/

	    /*for each current impurity, compute the number of pairs*/
	    for(j = 0; j < ndop; j++)  {
		s = dop[j];
		si = soltoimp[s];

                /* for now just use neutrals */
                kti = Kcpl[I][mat][si][CN];
                ktv = Kcpl[V][mat][si][CN];
                den = 1.0+kti*cs->nuevo[is][i]+ktv*cs->nuevo[vs][i];
                /* include the paired terms */
                a[is][s][i]  += cpl * cs->nco[i] * kti * cs->nuevo[is][i] *
                                (dact[s][s][i]/den);
                dtot1 += kti * ( (cs->newa[s][i])*(1.0+ktv*cs->nuevo[vs][i]) )/
                         (den*den);
                dtot2 += kti * ( (cs->newa[s][i])*(ktv*cs->nuevo[is][i]) )/
                         (den*den);
                cs->newa[is][i] += kti * cs->nuevo[is][i]*
                                   (cs->newa[s][i])/den;

		/*compute the equilibrium function times charge breakdown*/
/*		tmp = gprt[CN] * Kcpl(I,mat,si,CN)
			+ gprt[CP] * Kcpl(I,mat,si,CP)
			+ gprt[CDP] * Kcpl(I,mat,si,CDP)
			+ gprt[CTP] * Kcpl(I,mat,si,CTP)
			+ gprt[CM] * Kcpl(I,mat,si,CM)
			+ gprt[CDM] * Kcpl(I,mat,si,CDM)
			+ gprt[CTM] * Kcpl(I,mat,si,CTM);
*/
		/*derivitive w/r to the substitutional value*/
/*
		a[is][s][i]  += cpl * cs->nco[i] * tmp * cs->nuevo[is][i] * dequ[s][i];
		dtot += tmp * cs->newa[s][i];
		cs->newa[is][i] += tmp * cs->nuevo[is][i] * cs->newa[s][i];
*/
	    }
	}

	/*compute the appropriate term*/
	if ( cs->type == TR ) {
	    /*trapazoidal rule setup*/
            /* include pairing terms */
            a[is][is][i] += cpl * cs->nco[i] * dtot1;
            a[is][vs][i] -= cpl * cs->nco[i] * dtot2;
/*	    a[is][is][i] += cpl * cs->nco[i] * dtot; */

	    /*right hand side sum of flux terms*/
	    rhs[is][i] = newb[is][i] + oldb[is][i];

	    /*the time terms*/
	    rhs[is][i] -= cpl * ( cs->nco[i] * cs->newa[is][i] 
			      -   cs->oco[i] * cs->olda[is][i] );

	}
	else if ( cs->type == BDF ) {
            /* include pairing terms */
            a[is][is][i] += cpl * cs->nco[i] * dtot1;
            a[is][vs][i] -= cpl * cs->nco[i] * dtot2;
/*	    a[is][is][i] += cpl * cs->nco[i] * dtot; */

	    /*right hand side terms*/
	    rhs[is][i] = newb[is][i];
	    rhs[is][i] -=   cs->nco[i] * cpl * cs->newa[is][i] 
			  - cs->mco[i] * t2  * cs->mida[is][i]
			  + cs->oco[i] * t3  * cs->olda[is][i];

	}

	if ( (ts != -1) && ((mat == Poly) || (mat == Si) || (mat == GaAs)) ) {
	    double oa = cs->oco[i];
	    double na = cs->nco[i];

	    /*trap constants*/
	    es = Tfrac(mat) / (1.0 - Tfrac(mat));
	    kt = ktrap(I, mat);
	    bt = kt * equil[is][i] * es;
	    at = kt * cs->nuevo[is][i] + bt;
	    expa = exp( -at * cs->new_del );
	    tmp = bt / at;

	    /*compute the new trap concentration and right hand side*/
	    cs->nuevo[ts][i] = (expa * (ot[i]*oa - tmp*na) + tmp*na) / na;
	    dnda = (expa * (-cs->new_del*(ot[i] - tmp) + tmp / at) - tmp / at);
	    dndb = (1.0 - expa) / at;

	    if ( cs->type == TR ) {
		/*sum in the contribution of recombination*/
		jac = cpl * cs->nco[i] * Ttot(mat);
		a[is][is][i] -= jac * dnda * kt;
		a[is][ps][i] -= jac * (dndb + dnda) * kt * es * dequ[is][i];
		rhs[is][i] += cpl * Ttot(mat) * (cs->nco[i] * cs->nuevo[ts][i]
				  -  cs->oco[i] * cs->old[ts][i] );
	    }
	    else if ( cs->type == BDF ) {
		/*sum in the contribution of recombination*/
		jac = cpl * cs->nco[i] * Ttot(mat);
		a[is][is][i] -= jac * dnda * kt;
		a[is][ps][i] -= jac * (dndb + dnda) * kt * es * dequ[is][i];
		rhs[is][i] += Ttot(mat) * (cs->nco[i] * cpl * cs->nuevo[ts][i] 
			      - cs->mco[i] * t2  * cs->mid[ts][i]
			      + cs->oco[i] * t3  * cs->old[ts][i]);
	    }
	}

	/*clear out the defect rows - for non silicon and poly*/
	if ( (mat != Si) && (mat != Poly) && (mat != GaAs) ) {
	    clear_row( i, is );
	    rhs[is][i] = 0.0;
	    a[is][is][i] = 1.0;
	    cs->nuevo[is][i] = equil[is][i];
	}

	rhsnm[is] += rhs[is][i] * rhs[is][i];
	if ( a[is][is][i] == 0.0 ) a[is][is][i] = 1.0;
    }
}


/************************************************************************
 *									*
 *	Interstial( par, param ) - This routine gets the Interstial 	*
 *  diffusion parameters from the cards and sets them up. They 		*
 *  are then used by the rest of the program.				*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void interstitial(
char *par,
struct par_str* param)
{ 
    int mat, mat2 = -1;
    int imp = -1;
    char *tmp;

    /*get the material number specified*/
    if ( get_bool( param, "silicon" ) )	mat = Si;
    if ( get_bool( param, "oxide" ) )	mat = SiO2;
    if ( get_bool( param, "oxynitride" ) )	mat = OxNi;
    if ( get_bool( param, "poly" ) )	mat = Poly;
    if ( get_bool( param, "nitride" ) )	mat = SiNi;
    if ( get_bool( param, "gas" ) )	mat = GAS;
    if ( get_bool( param, "aluminum" ) )	mat = Al;
    if ( get_bool( param, "photoresist" ) )	mat = PhRs;
    if ( get_bool( param, "gaas" ) )	mat = GaAs;

    /*get the bulk values, if specified*/
    Fetch( D0(I,mat), "D.0" );
    Fetch( DE(I,mat), "D.E" );
    Fetch( Kr0(I,mat), "Kr.0" );
    Fetch( KrE(I,mat), "Kr.E" );
    Fetch( Cstar0(I,mat), "Cstar.0" );
    Fetch( CstarE(I,mat), "Cstar.E" );
    Fetch( ktrap0(I,mat), "ktrap.0" );
    Fetch( ktrapE(I,mat), "ktrap.E" );

    /*get the damage parameters, if specifies*/
    /*YASER 11/94                            */

    Fetch( globalalpha(I,mat), "glob.alpha" );
    Fetch( damalpha(I,mat), "local.alpha" );
    Fetch( damwidth(I,mat), "dam.width" );
    Fetch( damcenter(I,mat), "dam.center");

    /*get the interface material if given*/
    if ( Listed( "/silicon" ) )    mat2 = Si;
    if ( Listed( "/oxide" ) )      mat2 = SiO2;
    if ( Listed( "/oxynitride" ) ) mat2 = OxNi;
    if ( Listed( "/nitride" ) )    mat2 = SiNi;
    if ( Listed( "/poly" ) )       mat2 = Poly;
    if ( Listed( "/gas" ) )        mat2 = GAS;
    if ( Listed( "/gaas" ) )       mat2 = GaAs;

    /*if no second material listed, no interfce parameters can be*/
    if ( mat2 != -1 ) {

	/*work with the model specifiers*/ 
	if ( is_specified(param, "time.inj") ) {
	    if ( get_bool(param, "time.inj") )
		model(I,mat, mat2) = model(I,mat, mat2) | TIME;
	    else
		model(I,mat, mat2) = model(I,mat, mat2) & ~TIME;
	    model(I,mat2,mat) = model(I,mat,mat2);
	}
	if ( is_specified(param, "growth.inj") ) {
	    if ( get_bool(param, "growth.inj") )
		model(I,mat, mat2) = model(I,mat, mat2) | GROWTH;
	    else
		model(I,mat, mat2) = model(I,mat, mat2) & ~GROWTH;
	    model(I,mat2,mat) = model(I,mat,mat2);
	}
	if ( is_specified(param, "recomb") ) {
	    if ( get_bool(param, "recomb") )
		model(I,mat, mat2) = model(I,mat, mat2) | RECOM;
	    else
		model(I,mat, mat2) = model(I,mat, mat2) & ~RECOM;
	    model(I,mat2,mat) = model(I,mat,mat2);
	}
	if ( is_specified(param, "segregation") ) {
	    if ( get_bool(param, "segregation") )
		model(I,mat, mat2) = model(I,mat, mat2) | SEGRE;
	    else
		model(I,mat, mat2) = model(I,mat, mat2) & ~SEGRE;
	    model(I,mat2,mat) = model(I,mat,mat2);
	}

	/*work on the recombination parameters*/
	if ( is_specified( param, "Ksurf.0" ) ) {
	    Ksurf0(I,mat, mat2) = get_float( param, "Ksurf.0" );
	    Ksurf0(I,mat2, mat) = Ksurf0(I,mat, mat2);
	}
	if ( is_specified( param, "Ksurf.E" ) ) {
	    KsurfE(I,mat, mat2) = get_float( param, "Ksurf.E" );
	    KsurfE(I,mat2, mat) = KsurfE(I,mat, mat2);
	}
	if ( is_specified( param, "Krat.0" ) ) {
	    Krat0(I,mat, mat2) = get_float( param, "Krat.0" );
	    Krat0(I,mat2, mat) = Krat0(I,mat, mat2);
	}
	if ( is_specified( param, "Krat.E" ) ) {
	    KratE(I,mat, mat2) = get_float( param, "Krat.E" );
	    KratE(I,mat2, mat) = KratE(I,mat, mat2);
	}
	if ( is_specified( param, "Kpow.0" ) ) {
	    Kpow0(I,mat, mat2) = get_float( param, "Kpow.0" );
	    Kpow0(I,mat2, mat) = Kpow0(I,mat, mat2);
	}
	if ( is_specified( param, "Kpow.E" ) ) {
	    KpowE(I,mat, mat2) = get_float( param, "Kpow.E" );
	    KpowE(I,mat2, mat) = KpowE(I,mat, mat2);
	}

	/*time dependent injection parameters*/
	if ( is_specified( param, "A.0" ) ) {
	    A0(I,mat, mat2) = get_float( param, "A.0" );
	    A0(I,mat2, mat) = A0(I,mat, mat2);
	}
	if ( is_specified( param, "A.E" ) ) {
	    AE(I,mat, mat2) = get_float( param, "A.E" );
	    AE(I,mat2, mat) = AE(I,mat, mat2);
	}
	if ( is_specified( param, "t0.0" ) ) {
	    t00(I,mat, mat2) = get_float( param, "t0.0" );
	    t00(I,mat2, mat) = t00(I,mat, mat2);
	}
	if ( is_specified( param, "t0.E" ) ) {
	    t0E(I,mat, mat2) = get_float( param, "t0.E" );
	    t0E(I,mat2, mat) = t0E(I,mat, mat2);
	}
	if ( is_specified( param, "Tpow.0" ) ) {
	    Tpow0(I,mat, mat2) = get_float( param, "Tpow.0" );
	    Tpow0(I,mat2, mat) = Tpow0(I,mat, mat2);
	}
	if ( is_specified( param, "Tpow.E" ) ) {
	    TpowE(I,mat, mat2) = get_float( param, "Tpow.E" );
	    TpowE(I,mat2, mat) = TpowE(I,mat, mat2);
	}

	/*growth dependent injection parameters*/
	if ( is_specified( param, "vmole" ) ) {
	    vmole(I,mat, mat2) = get_float( param, "vmole" );
	    vmole(I,mat2, mat) = vmole(I,mat, mat2);
	}
	if ( is_specified( param, "theta.0" ) ) {
	    theta0(I,mat, mat2) = get_float( param, "theta.0" );
	    theta0(I,mat2, mat) = theta0(I,mat, mat2);
	}
	if ( is_specified( param, "theta.E" ) ) {
	    thetaE(I,mat, mat2) = get_float( param, "theta.E" );
	    thetaE(I,mat2, mat) = thetaE(I,mat, mat2);
	}
	if ( is_specified( param, "Gpow.0" ) ) {
	    Gpow0(I,mat, mat2) = get_float( param, "Gpow.0" );
	    Gpow0(I,mat2, mat) = Gpow0(I,mat, mat2);
	}
	if ( is_specified( param, "Gpow.E" ) ) {
	    GpowE(I,mat, mat2) = get_float( param, "Gpow.E" );
	    GpowE(I,mat2, mat) = GpowE(I,mat, mat2);
	}

	/*segregation parameters*/
	if ( is_specified( param, "Seg.0" ) ) {
	    Seg0(I,mat, mat2) = get_float( param, "Seg.0" );
	    Seg0(I,mat2, mat) = 1.0 / Seg0(I,mat, mat2);
	}
	if ( is_specified( param, "Seg.E" ) ) {
	    SegE(I,mat, mat2) = get_float( param, "Seg.E" );
	    SegE(I,mat2, mat) = - SegE(I,mat, mat2);
	}
	if ( is_specified( param, "Trn.0" ) ) {
	    Trn0(I,mat, mat2) = get_float( param, "Trn.0" );
	    Trn0(I,mat2, mat) = Trn0(I,mat, mat2);
	}
	if ( is_specified( param, "Trn.E" ) ) {
	    TrnE(I,mat, mat2) = get_float( param, "Trn.E" );
	    TrnE(I,mat2, mat) = TrnE(I,mat, mat2);
	}

	if ( is_specified( param, "rec.str" ) ) {
	    tmp = get_string( param, "rec.str" );
	    recstr(I,mat,mat2) = recstr(I,mat2,mat) = salloc(char, strlen(tmp)+1);
	    strcpy( recstr(I,mat,mat2), tmp );
	}
	if ( is_specified( param, "inj.str" ) ) {
	    tmp = get_string( param, "inj.str" );
	    injstr(I,mat,mat2) = injstr(I,mat2,mat) = salloc(char, strlen(tmp)+1);
	    strcpy( injstr(I,mat,mat2), tmp );
	}
    }

    /*Coupling coefficients with the impurities*/
    if ( Listed( "boron" ) )      imp = B;
    if ( Listed( "antimony" ) )   imp = Sb;
    if ( Listed( "arsenic" ) )    imp = As;
    if ( Listed( "phosphorus" ) ) imp = P;
    if ( Listed( "beryllium" ) )  imp = iBe;
    if ( Listed( "magnesium" ) )  imp = iMg;
    if ( Listed( "selenium" ) )   imp = iSe;
    if ( Listed( "isilicon" ) )	  imp = iSi;
    if ( Listed( "tin" ) )        imp = iSn;
    if ( Listed( "germanium" ) )  imp = iGe;
    if ( Listed( "zinc" ) )       imp = iZn;
    if ( Listed( "carbon" ) )     imp = iC;
    if ( Listed( "generic" ) )    imp = iG;

    /*if imp listed, than we have coupling coefficients*/
    if ( (imp != -1) ) {
        Fetch( Kcpl0[which(I)][mat][imp][CN], "neu.0" );
        Fetch( Kcpl0[which(I)][mat][imp][CM], "neg.0" );
        Fetch( Kcpl0[which(I)][mat][imp][CDM], "dneg.0" );
        Fetch( Kcpl0[which(I)][mat][imp][CP], "pos.0" );
        Fetch( Kcpl0[which(I)][mat][imp][CDP], "dpos.0" );
        Fetch( KcplE[which(I)][mat][imp][CN], "neu.E" );
        Fetch( KcplE[which(I)][mat][imp][CM], "neg.E" );
        Fetch( KcplE[which(I)][mat][imp][CDM], "dneg.E" );
        Fetch( KcplE[which(I)][mat][imp][CP], "pos.E" );
        Fetch( KcplE[which(I)][mat][imp][CDP], "dpos.E" );
/*
	Fetch( Kcpl(I,mat,imp,CN), "neu.0" );
	Fetch( Kcpl(I,mat,imp,CM), "neg.0" );
	Fetch( Kcpl(I,mat,imp,CDM), "dneg.0" );
	Fetch( Kcpl(I,mat,imp,CTM), "tneg.0" );
	Fetch( Kcpl(I,mat,imp,CP), "pos.0" );
	Fetch( Kcpl(I,mat,imp,CDP), "dpos.0" );
	Fetch( Kcpl(I,mat,imp,CTP), "tpos.0" );
*/
    }
    /*else we have the charge ratios*/
    else {
	/*get the charge ratios*/
	Fetch( Dfrac0[which(I)][mat][CN], "neu.0" );
	Fetch( Dfrac0[which(I)][mat][CM], "neg.0" );
	Fetch( Dfrac0[which(I)][mat][CDM], "dneg.0" );
	Fetch( Dfrac0[which(I)][mat][CTM], "tneg.0" );
	Fetch( Dfrac0[which(I)][mat][CP], "pos.0" );
	Fetch( Dfrac0[which(I)][mat][CDP], "dpos.0" );
	Fetch( Dfrac0[which(I)][mat][CTP], "tpos.0" );
	Fetch( DfracE[which(I)][mat][CN], "neu.E" );
	Fetch( DfracE[which(I)][mat][CM], "neg.E" );
	Fetch( DfracE[which(I)][mat][CDM], "dneg.E" );
	Fetch( DfracE[which(I)][mat][CTM], "tneg.E" );
	Fetch( DfracE[which(I)][mat][CP], "pos.E" );
	Fetch( DfracE[which(I)][mat][CDP], "dpos.E" );
	Fetch( DfracE[which(I)][mat][CTP], "tpos.E" );
    }
}


/************************************************************************
 *									*
 *	comp_intparam() - This routine computes the temperature 	*
 *  dependent interstitial parameters.					*
 *									*
 *  Original:	MEL	3/88						*
 *									*
 ************************************************************************/
void comp_intparam(
float temp)
{
    double Vt = kb * temp;
    register int mat1, mat2;
    register int wi = which(I);

    register int imp;


    /*for all the materials*/
    for(mat1 = 0; mat1 < MAXMAT; mat1++) {

	/*bulk parameters*/
	Di(I,mat1) = D0(I,mat1) * exp( - DE(I,mat1) / Vt );
	Kr(I,mat1) = Kr0(I,mat1) * exp( - KrE(I,mat1) / Vt );
	Cstar(I,mat1) = Cstar0(I,mat1) * exp( - CstarE(I,mat1) / Vt );
	ktrap(I,mat1) = ktrap0(I,mat1) * exp( - ktrapE(I,mat1) / Vt );
	Tfrac(mat1) = Tfrac0(mat1) * exp( - TfracE(mat1) / Vt );
	Dfrac[wi][mat1][CN] = Dfrac0[wi][mat1][CN]*exp(-DfracE[wi][mat1][CN]/Vt);
	Dfrac[wi][mat1][CM] = Dfrac0[wi][mat1][CM]*exp(-DfracE[wi][mat1][CM]/Vt);
	Dfrac[wi][mat1][CDM] = Dfrac0[wi][mat1][CDM]*exp(-DfracE[wi][mat1][CDM]/Vt);
	Dfrac[wi][mat1][CTM] = Dfrac0[wi][mat1][CTM]*exp(-DfracE[wi][mat1][CTM]/Vt);
	Dfrac[wi][mat1][CP] = Dfrac0[wi][mat1][CP]*exp(-DfracE[wi][mat1][CP]/Vt);
	Dfrac[wi][mat1][CDP] = Dfrac0[wi][mat1][CDP]*exp(-DfracE[wi][mat1][CDP]/Vt);
	Dfrac[wi][mat1][CTP] = Dfrac0[wi][mat1][CTP]*exp(-DfracE[wi][mat1][CTP]/Vt);

        for(imp = 0; imp < MAXIMP; imp++) {
            Kcpl[wi][mat1][imp][CN] = Kcpl0[wi][mat1][imp][CN]*
                                      exp(-KcplE[wi][mat1][imp][CN]/Vt);
            Kcpl[wi][mat1][imp][CM] = Kcpl0[wi][mat1][imp][CM]*
                                      exp(-KcplE[wi][mat1][imp][CM]/Vt);
            Kcpl[wi][mat1][imp][CDM] = Kcpl0[wi][mat1][imp][CDM]*
                                       exp(-KcplE[wi][mat1][imp][CDM]/Vt);
            Kcpl[wi][mat1][imp][CP] = Kcpl0[wi][mat1][imp][CP]*
                                      exp(-KcplE[wi][mat1][imp][CP]/Vt);
            Kcpl[wi][mat1][imp][CDP] = Kcpl0[wi][mat1][imp][CDP]*
                                       exp(-KcplE[wi][mat1][imp][CDP]/Vt);
        }

	/*interface terms*/
	for(mat2 = 0; mat2 < MAXMAT; mat2++) {

	    /*surface recombination*/
	    Ksurf(I,mat1,mat2) = Ksurf0(I,mat1,mat2) * exp( - KsurfE(I,mat1,mat2) / Vt );
	    Krat(I,mat1,mat2) = Krat0(I,mat1,mat2) * exp( - KratE(I,mat1,mat2) / Vt );
	    Kpow(I,mat1,mat2) = Kpow0(I,mat1,mat2) * exp( - KpowE(I,mat1,mat2) / Vt );

	    /*growth model for injection*/
	    theta(I,mat1,mat2) = theta0(I,mat1,mat2) * exp( - thetaE(I,mat1,mat2) / Vt );
	    Gpow(I,mat1,mat2) = Gpow0(I,mat1,mat2) * exp( - GpowE(I,mat1,mat2) / Vt );
	    
	    /*time dependent injection model*/
	    A(I,mat1,mat2) = A0(I,mat1,mat2) * exp( - AE(I,mat1,mat2) / Vt );
	    t0(I,mat1,mat2) = t00(I,mat1,mat2) * exp( - t0E(I,mat1,mat2) / Vt );
	    Tpow(I,mat1,mat2) = Tpow0(I,mat1,mat2) * exp( - TpowE(I,mat1,mat2) / Vt );

	    /*segregation terms*/
	    Seg(I,mat1,mat2) = Seg0(I,mat1,mat2) * exp( - SegE(I,mat1,mat2) / Vt );
	    Trn(I,mat1,mat2) = Trn0(I,mat1,mat2) * exp( - TrnE(I,mat1,mat2) / Vt );
	}
    }
}


float form_eval(
char *expr,
float total,
float cord[2])
{
    float val;

    fmacro( "t", "%e", total );
    fmacro( "x", "%e", cord[0]*1.0e4 );
    fmacro( "y", "%e", cord[1]*1.0e4 );

    val = string_to_real( expr, -1.0 );
    if ( val < 0 ) {
	fprintf(stderr, "problems with the formula %s", expr);
	val = 0;
    }

    umacro( "t" );
    umacro( "x" );
    umacro( "y" );
    return( val );
}

#ifdef HUANG
/************************************************************************
 *                                                                      *
 *      Isink( ) - This routine computes the growth of dislocation loops*
 *  as well as the recombination of interstitials with extended defects.*
 *                                                                      *
 *  Original:   RYSH     8/94 (based on 9130 version written 3/93)      *
 *                                                                      *
 ************************************************************************/
void Isink(
float temp,
double *area,           /*the nodal areas*/
double **znew,          /*all the concentrations*/
double **equil,         /*the equilibrium concentration*/
double **dequ,          /*the derivitive of equilibrium concentration*/
double **rhs            /*the current right hand side*/)
{
    register int is = imptosol[I];
    register int ps = imptosol[Psi];
    register int i, mat;
    double currentci, newcistar;/* current CI and new CI*'(r) */
    double cpl, t1, t2;
    double del_r;               /* increase in loop radius */
    double No=1.5665e15;        /* (111) planar density of silicon */

    /* if loops are smaller than 10 Angstroms in radius, ignore them */

  if (loopr > 1.0e-7) {

    /* Knowing looploc, we can find CI at this location */
    for (i = 0;i < nn; i++) {
        if (mode == TWOD) {
            t1 = pt[ nd[i]->pt ]->cord[1];
        }
        else {
            t1 = pt[ nd[i]->pt ]->cord[0];
        }

        if (fabs(t1-looploc) < 1.0e-8) {
            /* get interstitial concentration at center location of loops */
            currentci = znew[is][i];
            newcistar = equil[is][i];
        }
    }

    /* first calculate CI*', equilibrium CI around loops of radius r */

    t2 = exp( ( (mu*burger*omega)/((4.0*PI*loopr*kb*temp)*(1.0-nu)) )*
         log(8.0*loopr/ro)+(disgamma*omega)/(burger*kb*temp) );

    /* If CI*'/CI* is larger than maximum user-defined si, set value to
       user-defined si */

    if (t2 >= maxsi)
        t2 = maxsi;

    /* calculate change in loop radius */
    /* This equation assumes the reaction depends on (CI-CI*') and a reaction
       coefficient, fdrdt */
    /* Use Hu's formula from 1981 reference in "Defects in Semiconductors"
       fdrdt now represents the factor (r_c/r_a)^2*exp(-deltaH/kT) */
    del_r = (PI*Di(I,Si)*fdrdt*newcistar*(currentci/newcistar-t2)*loopdel_t)/No;

    looprnew = loopr + del_r;

    /* Use this when calculating density changes (dl/dt = lfactor*dr/dt) */
    looplengthnew = looplength + lfactor*del_r;

    /*Now calculate the recombination if we're within the radius of the loop*/
    /*and if del_r != 0.0*/

    if (del_r != 0.0) {

        /*for each node, compute the recombination*/
        for(i = 0; i < nn; i++) {

            /* calculate location */
            if (mode == TWOD) {
                t1 = pt[ nd[i]->pt ]->cord[1];
            }
            else {
                t1 = pt[ nd[i]->pt ]->cord[0];
            }

            /* compute the coupling terms */
            cpl = sqrt(1.5)*PI*PI*rho*Di(I,Si)*fdrdt*area[i]*
                  (1.0-((lfactor*loopr)/looplength));

            /* activate sink if we're in the right location */
            if (fabs(t1-looploc) < loopr/sqrt(1.5)) {

                /*calculate left hand side terms*/
                a[is][is][i] += cpl;
                a[is][ps][i] -= cpl * dequ[is][i] * t2;

                /*calculate right hand side terms at new point*/
                rhs[is][i] -= cpl * newcistar * (znew[is][i]/newcistar - t2);
            }
        }
    }
  }
}
#endif


/************************************************************************
 *                                                                      *
 *      Isink( ) - This routine computes the recombination of           *
 *  interstitials with extended defects.                                *
 *                                                                      *
 *  Original:   RYSH    7/92                                            *
 *  Modified:   YMH 4/93                                                *
 *                                                                      *
 ************************************************************************/
void Isink( 
float temp,
double *area,           /*the nodal areas*/
double **znew,           /*all the concentrations*/
double **equil,         /*the equilibrium concentration*/
double **dequ,          /*the derivitive of equilibrium concentration*/
double **rhs)           /*the current right hand side*/
{
    register int is = imptosol[I];
    register int ps = imptosol[Psi];
    register int i, mat;
    double cpl, t1;

        /*for each node, compute the recombination*/
    for(i = 0; i < nn; i++) {

      /*which material are we working with*/
      mat = nd[i]->mater;

        /* calculate location */
        t1 = pt[ nd[i]->pt ]->cord[1];  /* default: y coordinate is cord[1] */
        if (mode == ONED)
           t1 = pt[ nd[i]->pt ]->cord[0]; /* for 1D, y coordinate is cord[0]*/

        /* compute the global defect coupling terms */
        cpl = globalalpha(I,mat) * area[i];

        /* activate global sink/source */

           /* 1. Modify LHS terms */
           a[is][is][i] += cpl;
           a[is][ps][i] -= cpl * dequ[is][i];

           /* 2. Modify RHS term */
           rhs[is][i] -= cpl* (znew[is][i] - equil[is][i]);


        /* compute the local defect coupling terms */
        cpl = damalpha(I,mat) * area[i];

        /* activate sink/source if we're in the right location */
        if (fabs(t1-damcenter(I,mat)) < damwidth(I,mat)) {

            /* 1. Modify left hand side terms*/
            a[is][is][i] += cpl;
            a[is][ps][i] -= cpl * dequ[is][i];

            /* 2. Modify right hand side terms at new point*/
            rhs[is][i] -= cpl * (znew[is][i] - equil[is][i]);
        }
    }
}


