/*   list.c                Version 1.5     */
/*   Last Modification : 7/21/94 15:42:34        */
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

#include <malloc.h>
#include <stdio.h>
#include "global.h"
#include "constant.h"
#include "dbaccess.h"
#include "misc.h"

/*
 *	Add to a list.
 */
void add_list(
struct list_str *l,
int item)
{
	
    /*check for space consideration*/
    if ( l->num >= l->all ) {
	if ( l->all == 0 ) {
	    l->all = 10;
	    l->list = salloc( int, l->all );
	}
	else {
	    l->all += 10;
	    l->list = sralloc(int, l->all, l->list);
	}
    }

    /*store the triangle number away*/
    l->list[ l->num++] = item;
}


void dis_list(
struct list_str *l)
{
    if (l->list) free((char *) l->list);
    l->list = NULL;
    l->num = 0;
    l->all = 0;
}

void pr_list(
struct list_str *l)
{
    int i;

    printf("    ");
    for(i = 0; i < l->num; i++) printf("%d ", l->list[i]);
    printf("\n");
}



void sub_list(
struct list_str *l,
int item)
{
    int i, j;

    for(i = 0; (i < l->num) && (l->list[i] != item); i++);

    if ( i == l->num ) 
	panic("subtracting nonexistant value from list");
    
    for(j = i; j < l->num-1; j++) l->list[j] = l->list[j+1];

    l->num--;
}

void sub_nd_pt(
int n, int p)
{
    int i, j;

    for(i = 0; (i < pt[p]->nn) && (pt[p]->nd[i] != n); i++);

    if ( i == pt[p]->nn ) 
	panic("subtracting nonexistant node from point");
    
    for(j = i; j < pt[p]->nn; j++) pt[p]->nd[j] = pt[p]->nd[j+1];

    pt[p]->nn--;
}

