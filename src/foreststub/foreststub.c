/*   foreststub.c                Version 1.1     */
/*   Last Modification : 8/29/94 10:43:10        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/

#include "forest.h"

/* 
 *  Read in a geometry, and generate a grid.
 */
int forest(char *filename, int suprem_input, int whichsimulator, int firsttime)
{
 return (0);
}


/* 
 *  Talk with SUPREM IV diffusion
 */
void Forest2Diffuse( void )
{
}

void Diffuse2Forest( int disallflag )
{
}


void ForestGetGeometryLimits(double *xmi, double *ymi, double *xma, double *yma)
{
}



/* 
 *  Stuff for oxidation
 */
int ForestMoveNodes(double deltatime, int regrid)
{
 return (0);
}

double ForestGridDt(double maxtime)
{
 return (0.0);
}


/* 
 *  Grid adaptation
 */
int GridAdapt(int refineflag, int coarsenflag)
{
 return (0);
}


/* 
 *  Other services provided by Forest
 */
void ForestDeposit(int materialnumber, double thickness, double gridsize,
                   int impuritynumber, double impurityconc)
{
}


void ForestEtch(int materialnumber)
{
}

void ForestEtchCoord(int materialnumber, double *xcoord, double *ycoord, int numpt)
{
}


void ForestAfterSpeedie( void )
{
}


#ifdef _cplusplus
extern "C"
#endif
void Forest2Speedie(double maxedgelen)
{
}


#ifdef _cplusplus
extern "C"
#endif
void Speedie2Forest(double *dTimeGranted, double dTime)
{
}


