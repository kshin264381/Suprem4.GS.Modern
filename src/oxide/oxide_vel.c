/*   oxide_vel.c                Version 1.6     */
/*   Last Modification : 8/29/94 10:51:48        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
/*-----------------OXIDE_VEL--------------------------------------------
 * This subroutine contains the oxide growth rate model.  It uses global
 * parameters like temperature and local parameters like oxidant conc to
 * compute the displacement at each interface point. Then it calls the
 * oxide solver to compute the internal oxide displacements.  The result is
 * returned as a vector of nodal displacements. The silicon nodes and oxide
 * nodes on the interface get displaced by opposite amounts in the
 * proportions of 1.2 :  1.
 *
 *----------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "global.h"
#include "constant.h"
#include "material.h"
#include "impurity.h"
#include "diffuse.h"
#include "geom.h"
#include "dbase.h"
#include "FEgeom.h"
#include "oxide.h"
#include "finel.h"

double temperature;		/* I'm tired of passing this guy around */

void SetFuse(void);

void noop(
	  double *stiff, /* The stiffness matrix */
	  double *wrhs,       /* extra right hand side stuff */
	  double *xl,         /* Coordinates of nodes */
	  double *disp,       /* The current solution/displacement */
	  void (*coeff)(double *, int, double *, double *),           /* Fetches material data */
	  int mat,                   /* What material this triangle is */
	  int *fixity,        /* The nodal boundary codes */
	  double *bc,         /* Any nodal boundary float info */
	  int ie,                    /* because all this is still not enough */
	  int fixme)                 /* a variable of unsurpassed ugliness */
{
 return;
}


#define NEL 7			/* number of nodes in triox */


/*-----------------OXIDE_VEL--------------------------------------------
 * Now just a switching station.
 *----------------------------------------------------------------------*/
void oxide_vel (
    float temp,		/* Processing temperature */
    int ornt,		/* Orientation of substrate */
    int oxhow,		/* Dry or wet oxidation */
    double dt)
{
    temperature = temp;

    /*load the finite element structure*/
    FEdesc[0].nel = NEL;
    FEdesc[0].gaussp = 3;
    FEdesc[0].stiff = triox_stiff;
    FEdesc[0].coeff = FE_oxbulk;
    FEdesc[0].bound = triox_bc;
    FEdesc[0].nodal_stress = triox_nodal_stress;

    FEdesc[1].nel = NEL;
    FEdesc[1].gaussp = 3;
    FEdesc[1].stiff = tri6_stiff;
    FEdesc[1].coeff = sup4_ecoeff;
    FEdesc[1].bound = noop;
    FEdesc[1].nodal_stress = tri6_Snodal_stress;

    FEdesc[2].nel = 7;
    FEdesc[2].gaussp = 3;
    FEdesc[2].stiff = tri7_stiff;
    FEdesc[2].coeff = sup4_ecoeff;
    FEdesc[2].bound = noop;
    FEdesc[2].nodal_stress = tri7_nodal_stress;

    FEdesc[3].nel = NEL;
    FEdesc[3].gaussp = 0;
    FEdesc[3].stiff = NULL;
    FEdesc[3].coeff = NULL;
    FEdesc[3].bound = NULL;
    FEdesc[3].nodal_stress = NULL;

    FEdesc[4].nel = 7;
    FEdesc[4].gaussp = 3;
    FEdesc[4].stiff = tri7_stiff;
    FEdesc[4].coeff = sup4_ecoeff;
    FEdesc[4].bound = oxload;
    FEdesc[4].nodal_stress = tri7_nodal_stress;

    /* Don't wipe velocities unless we're sure to calculate new ones. */
    switch (oxide_model) {
	case O_ANALYTIC: {WipeVel(); erfc_growth( temp, ornt, oxhow, dt);}
		break;
        case O_ERF1:
        case O_ERF2:
        case O_ERFG:     {WipeVel(); erf1_growth( temp, ornt, oxhow, dt, oxide_model);}
                break;
	case O_VERTICAL: {WipeVel(); vert_growth( temp);}
		break;
	case O_ELASTIC:
	  if( NeedNewFlow( )) 
	    {
	     WipeVel(); 
	     elast_growth( temp, dt); 
	     SetFuse();
	    }
	  break;

	case O_VISCOUS:
	  if( NeedNewFlow( )) 
	    {
	     WipeVel(); 
	     visco_growth( temp, dt); 
	     SetFuse();
	    }
	  break;
	default:
	  fprintf( stderr, "Unknown oxide model!\n");
	  WipeVel();
	  break;
    }
}
/*-----------------Onvel------------------------------------------------
 * Wrapper routine for Ovel.
 *----------------------------------------------------------------------*/
void Onvel(
    int io,			/* Oxide node number */
    float vel[2])		/* Returned velocity */
{
    int Ps = imptosol[Psi];
    int is; double myNoni; double conc;
    float vell;
    double dln[2];		/* local normal for Ovel */
    double Kt = KB * temperature;

    /* Get the normal */
    (void)dlocal_normal( io, Si, dln);
    
    if (nd[ io]->mater != SiO2)
	return;
    else {
	is = node_mat( io, Si);	assert(is>=0);
	myNoni = exp( nd[ is]->sol[Ps] / Kt );
	conc = nd[io]->sol[imptosol[gas_type]];
	vell = Ovel (temperature, gas_type, conc, dln, myNoni);
	vel[0] = vell * dln[0];
	vel[1] = vell * dln[1];
    }
    return;
}


/************************************************************************
 * This page determines when new oxide flow calculations are necessary.	*
 * The idea is to avoid recomputing the flow as often as diffusion in	*
 * the substrate is solved, because the substrate may do dozens of	*
 * itty-bitty diffusion steps for each oxidation step.			*
 * The decision is based on whether it's been long enough since		*
 * the last solution, where "long enough" means a fraction of the grid	*
 * motion time, gdt.							*
 ************************************************************************/

/*-----------------NeedNewFlow()--------------------------------------
 *----------------------------------------------------------------------*/
static float fuse=0.0;
int NeedNewFlow( )
{
#ifdef OLDSTUFF
    /* If it's the first step, or if we've passed the time marker: */
    if( total == 0 || total >= fuse) {
	return(1);
    }
    else {
	return(0);
    }
#endif
    
    return (1);
}
void SetFuse()
{
    extern double grid_dt();

    fuse = total + oxide_redo * grid_dt();
}


/*-----------------WipeVel----------------------------------------------
 * Wipe the slate clean before starting again.
 *----------------------------------------------------------------------*/
void WipeVel()
{
    int in;
    
    /* Initialize the displacement vector to 0 */
    for (in = 0; in < nn; in++)
	nd[in]->sol[imptosol[XVEL]] = nd[in]->sol[imptosol[YVEL]] = 0;
}

