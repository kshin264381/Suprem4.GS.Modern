/*   dislocation.c                Version 1.2     */
/*   Last Modification : 8/31/94 14:25:18        */
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

#include "global.h"
#include "check.h"
#include "dislocation.h"
#include "misc.h"

/************************************************************************
 *									*
 *	dislocation( par, param ) - This routine gets the parameters 	*
 *  needed to solve for the growth and shrinkage of dislocation loops   *
 *									*
 *  Original:	RYSH	8/94 (from 9130 code written 3/93)		*
 *									*
 ************************************************************************/
void dislocation(
char *par,
struct par_str* param)
{
    if ( is_specified(param, "rinit") ) {
        rinit = get_float( param, "rinit");
        loopr = rinit;
    }

    if ( is_specified( param, "ro" ) )
        ro = get_float( param, "ro");

    if ( is_specified( param, "looploc" ) )
        looploc = get_float( param, "looploc");

    if ( is_specified( param, "rho" ) ) {
        rho = get_float( param, "rho");
/*      Use this if including change in density */
        looplength = sqrt(0.5/rho);
    }

    if ( is_specified( param, "burger" ) )
        burger = get_float( param, "burger");

    if ( is_specified( param, "fdrdt" ) )
        fdrdt = get_float( param, "fdrdt");

    if ( is_specified( param, "loopgdt" ) )
        loopgdt = get_float( param, "loopgdt");

    if ( is_specified( param, "lfactor" ) )
        lfactor = get_float( param, "lfactor");

    if ( is_specified( param, "maxsi" ) ) {
        maxsi = get_float( param, "maxsi");
    }

    if ( is_specified( param, "gamma" ) )
        disgamma = get_float( param, "gamma");

    if ( is_specified( param, "omega" ) )
        omega = get_float( param, "omega");

    if ( is_specified( param, "mu" ) )
        mu = get_float( param, "mu");

    if ( is_specified( param, "nu" ) )
        nu = get_float( param, "nu");

}
