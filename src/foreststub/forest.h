/*   forest.h                Version 2.12     */
/*   Last Modification : 8/5/94 13:43:08        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
/* Author:  Zakir Sahul                                            */
/*-----------------------------------------------------------------*/

#ifndef ForestH
#define ForestH

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

/* 
 *  Read in a geometry, and generate a grid.
 */
int forest(char *filename, int suprem_input, int whichsimulator, int firsttime);
#define SIMULATOR_SUPREM 0
#define SIMULATOR_PISCES 1
#define SIMULATOR_SPEEDIE 2

/* 
 *  Talk with SUPREM IV diffusion
 */
void Forest2Diffuse( void );
void Diffuse2Forest( int disallflag );

void ForestGetGeometryLimits(double *xmi, double *ymi, double *xma, double *yma);


/* 
 *  Stuff for oxidation
 */
int ForestMoveNodes(double deltatime, int regrid);
double ForestGridDt(double maxtime);

/* 
 *  Grid adaptation
 */
int GridAdapt(int refineflag, int coarsenflag);

/* 
 *  Other services provided by Forest
 */
void ForestDeposit(int materialnumber, double thickness, double gridsize,
                   int impuritynumber, double impurityconc);

void ForestEtch(int materialnumber);
void ForestEtchCoord(int materialnumber, double *xcoord, double *ycoord, int numpt);

void ForestAfterSpeedie( void );

#endif
