/*   dislocation.h                Version 1.2     */
/*   Last Modification : 8/31/94 14:24:46        */
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

#ifndef __DISLOCATION_H__
#define __DISLOCATION_H__

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

EXTERN double rinit;            /* initial radius of dislocation loops */
EXTERN double ro;               /* core radius used in calculation of new CI* */
EXTERN double looploc;          /* center location of dislocation loops */
EXTERN double rho;              /* density of dislocation loops in atoms/cm^2 */
EXTERN double burger;           /* magnitude of Burger's vector of dislocation
                                   loop */
EXTERN double fdrdt;            /* fitting parameter for dr/dt equation */
EXTERN double loopr;            /* current radius of dislocation loops */
EXTERN double looprnew;         /* temporary variable for new radius of
                                   dislocation loops */
EXTERN double loopdel_t;        /* current timestep */
EXTERN double loopgdt;          /* used for calculation of maximum timestep */

/* Use these if including density changes */
EXTERN double looplength;       /* calculated from rho, rho = 1/(2L^2)  */
EXTERN double looplengthnew;    /* temporary variable for new L */
EXTERN double lfactor;          /* dl/dt = lfactor * dr/dt */
EXTERN double maxsi;		/* maximum user-defined CI*'/CI* */
EXTERN double disgamma;         /* Internal energy of stacking fault */
EXTERN double omega;            /* Volume per silicon atom */
EXTERN double mu;               /* Shear modulus of silicon */
EXTERN double nu;               /* Poisson's ratio for silicon */

#endif /* __DISLOCATION_H__ */
