/*   tomcal.c                Version 1.7     */
/*   Last Modification : 8/29/94 10:42:25        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#include "global.h"
#include "misc.h"


void tomcal(
    int *ia,
    int *reord,
    int *lvls)
{
    int neq, i, nnb, *adjstk, *check;

    /* Find how big to make the array by getting max num nbrs */
    neq = ia[0]-1;

    for (nnb = 0, i = 0; i < neq; i++)
	if (ia[i+1] - ia[i] > nnb)
	    nnb = ia[i+1]-ia[i];

    /* OK, malloc off enough storage */
    adjstk = salloc( int, (neq+1)*(nnb+2));

    nnb++;
#ifdef ardent
    if(TOMCAL( &neq, &nnb, ia, reord, adjstk, lvls)<0) abort();
#else
#   ifdef NO_F77
#   else
    if(tomcal_( &neq, &nnb, ia, reord, adjstk, lvls)<0) abort();
#   endif
#endif

    /* let's be careful */
    check = salloc(int, neq);
    for (i = 0; i < neq; i++) check[i] = 0;
    for (i = 0; i < neq; i++) check[reord[i]]++;
    for (i = 0; i < neq; i++) if (check[i] != 1) abort();
    free((char *)check);
    free((char *)adjstk);
}
