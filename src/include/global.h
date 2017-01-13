/*************************************************************************
 *									 *
 *   Original : MEL         Stanford University        Sept, 1984	 *
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   global.h                Version 5.1     */
/*   Last Modification : 7/3/91  08:55:06 */

#ifndef GlobalH
#define GlobalH

/*#include "malloc.h"*/

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

#define TRUE 1
#define FALSE 0

/* Erase or comment this line if you wish to debug X11 part */
//#define __noplot__

/***************************************************
 *                                                 *
 *            Standard variables.                  *
 *                                                 *
 ***************************************************/
#define V_QUIET  10
#define V_NORMAL 100
#define V_CHAT   1000
#define V_BARF   10000
EXTERN int verbose;

/* 
 *  this global data structure is used for simple flag
 *  type options.
 */
struct S7OptionsStruct
{
#define SUPREM_GRIDDER 0
#define FOREST_GRIDDER 1
    int gridder;                /* what gridder should we use? */
    int implant_grid_adapt;	    /* adapt grid during implantation simulation? */
    int diffuse_grid_adapt;	    /* adapt grid during diffusion simulation? */
    int oxidation_grid_adapt;   /* adapt grid during oxidation? */
};

EXTERN struct S7OptionsStruct S7Options;

/***************************************************
 *                                                 *
 *            Standard macros.                     *
 *                                                 *
 ***************************************************/
#define FOR_RNG(i,l,u) for(i = (l); i <= (u); i++)
#define max(a,b) (((a) > (b))?(a):(b))
#define min(a,b) (((a) < (b))?(a):(b))
#define strequ(a,b) (!strcmp(a,b))

EXTERN char *MallocResult;
#define salloc(O,N) (O *)( MallocResult =  (char *)malloc((unsigned)((N)*sizeof(O)))); if (MallocResult == NULL) panic("Out of memory");
#define scalloc(O,N) (O *) ((MallocResult =  (char *)calloc((unsigned)(N),(unsigned)(sizeof(O))))); if (MallocResult == NULL) panic("Out of memory");
#define sralloc(O,N,P) (O *) ((MallocResult = (char *)realloc((char *)P, (unsigned)((N)*sizeof(O))))); if (MallocResult == NULL) panic("Out of memory");

#define sfree(P) free((char*)P)

/***************************************************
 *                                                 *
 *       Standard procedures and defines.          *
 *                                                 *
 ***************************************************/

extern int mod();

#define Fetch( V, S ) if (is_specified(param,S)) V=get_float(param,S)
#define Listed( S ) ( get_bool(param,S) && is_specified(param,S) )

/***************************************************
 *                                                 *
 *              Operating Mode			   *
 *                                                 *
 ***************************************************/
#define ONED 1
#define TWOD 2
EXTERN int mode;


#endif
