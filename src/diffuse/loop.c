/*   loop.c                Version 1.2     */
/*   Last Modification : 8/31/94 14:25:19        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
/************************************************************************
 *
 *    Calculate minimum time step due to dislocation loop growth        *
 *
 ************************************************************************/

#include <math.h>

#include "global.h"
#include "dislocation.h"
#include "impurity.h"
#include "geom.h"
#include "constant.h"
#include "diffuse.h"
#include "material.h"
#include "defect.h"

double do_disloop(
float temp,
double *dt) 
{
    register int is = imptosol[I];
    register int i;
    double newcistar, t1, t2;
    double maxgradient;         /*maximum driving force for loop growth*/
    double maxdel;              /*maximum delta for loop growth*/
    double No=1.5665e15;        /* (111) planar density of silicon */

    maxgradient = 0.0;

    /*calculate the maximum CI-CI*', which is driving force for loop growth */
    for(i = 0; i < nn; i++) {

        /* calculate location */
        if (mode == TWOD) {
            t1 = pt[ nd[i]->pt ]->cord[1];
        }
        else {
            t1 = pt[ nd[i]->pt ]->cord[0];
        }

        if (fabs(t1-looploc) < loopr/sqrt(1.5)) {
            /* calculate CI*' at this point */
            newcistar = equil[is][i];
            /* update expression for CI*' */
            t2 = exp( ( (mu*burger*omega)/((4.0*PI*loopr*kb*temp)*(1.0-nu)) )*
                 log(8.0*loopr/ro)+(disgamma*omega)/(burger*kb*temp) );
                if (t2 >= maxsi)
                    t2 = maxsi;
            newcistar *= t2;

            /* if |(CI-CI*')| greater than any previous one, set maxgradient */
            if ( fabs(new_sol[is][i] - newcistar) > maxgradient )
                maxgradient = fabs(new_sol[is][i] - newcistar);
        }

    }

    /* may need to put in something if maxgradient = 0.0 */

    /* calculate maximum delta */
    if (maxgradient < EPS) {
        maxdel = MAXFLOAT;
    }
    else {
        maxdel = (loopr * loopgdt*No)/(PI*Di(I,Si)*fdrdt*maxgradient);
    }
    return( maxdel);

}
