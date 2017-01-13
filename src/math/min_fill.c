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
/*   min_fill.c                Version 5.1     */
/*   Last Modification : 7/3/91 10:44:15 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "global.h"
#include "constant.h"
#include "geom.h"
#include "dbase.h"
#include "material.h"
#include "misc.h"
#include "local_math.h"

/*#define salloc(S,N) (S *) malloc( (N)*sizeof(S))*/
    /*printf("allocated N S's = %d bytes\n", (N)*sizeof(S))*/

/*#define sralloc(Old,Type,Num) (Type *) realloc( Old, Num * sizeof(Type))*/
    /*printf("reallocated Num Type's = %d bytes\n", (Num)*sizeof(Type))*/

struct d_sort {
	float x;
	int nbin;
	int mat;
};

/* All links come off a single big list */
/* This is defined in local_math.h
 struct _link 
{
 struct _link *n; 
 int v;
};

typedef struct _link link;
*/

link_t *storage, *freelist;
int   storage_size;
int   su, hwm;			/* Storage used, high water mark */

/* link_t manipulators */
// --> changed to function form to actually track stuff.
//#define takefrom( F, O) {link_t *t = (O); (O) = (F); (F) = (F)->n; (O)->n = t;}
//#define new_link(A) {if (!freelist) new_storage(); takefrom( freelist, A); if (++su > hwm) hwm = su;}
//#define free_link(A) {takefrom((A)->n, freelist); su--;}

void takefrom(link_t** F, link_t** O)
{
	link_t* tmp = *O;
	*O = *F;
	*F = (*F)->n;
	(*O)->n = tmp;
}

void new_link(link_t** A)
{
	if (!freelist) new_storage();
	takefrom(&freelist, A);
	if (++su > hwm) hwm = su;
}

void free_link(link_t** A)
{
	takefrom(&((*A)->n), &freelist);
	su--;
}
	
/************************************************************************
 *									*
 *	min_ia_fill( ia, reorder, talk) - Find a minimum fill ordering. *
 *									*
 *  Original:	MEL	11/84						*
 *  Modified:   CSR     09/86 No inkludge files, thanks			*
 *  Rewritten:	CSR	02/86 Don't give up until malloc does		*
 *	        LCC	06/90 Add 1-D mode
 *									*
 * the idea here is to do a mock gauss elimantion, updating the pt to pt*
 * list at each step.  At each step, a decision is made about the	*
 * optimal node to work on next. Returns size of strict upper triangle.	*
 ************************************************************************/
int min_ia_fill(
	int *ia,			/* Matrix map including l and u */
	int *reorder,		/* Result vector */
	int talk)
{
    link_t *nbrs;			/* Base of each nodes nbr list: .v=length */
    int in, nn, j;

    struct d_sort *order;	     
    int count=0;
    float tmp;

    int i, best, besti, bestn;	/* Best is node being eliminated */
    int *remaining, nremain;	/* Array of uneliminated nodes */
    int *mask;			/* Tmp for computing union */
    int work;			/* neighbor of best being worked on */
    link_t *p, *workl, *bestl;
    link_t *q, *lastq;
    int  nnb, step, Obestn = -1, total = 0, e;

    /* Initialize storage - make a guess of l ~ 5*a, update later.*/
    nn = ia[0] - 1;

    order = salloc( struct d_sort, nn );

    if ( mode == TWOD ) {
        mask = salloc( int, nn);
        remaining = salloc( int, nn);
    }

    nbrs = salloc( link_t, nn);
    storage_size = 2*ia[ nn];
    su = hwm = 0;
    storage = NULL;
    new_storage();

    /* Initialize link table */
    for (in = 0; in < nn; in++) {

    	/* 1-D we store data in ORDER and qsort it ! */
		if ( mode == ONED ) {
			order[in].x = pt[ nd[in]->pt ]->cord[0];
			order[in].nbin = in;
			order[in].mat  = nd[in]->mater;
			count++;
		}
		else {
			mask[in] = 0;
			remaining[in] = in;
		}

		nbrs[in].v = ia[in+1] - ia[in];
		for (nbrs[in].n = NULL, j = ia[in]; j < ia[in+1]; j++) {
			new_link(&nbrs[in].n);
			nbrs[in].n->v = ia[j];
		}
    }

    /*Switch on the mode*/
    switch ( mode ) {
    case  ONED :

	/*do a linear search to find an exposed surface node*/
	for(best = -1, i = 0; i<ne && best < 0; i++) {
	    if (tri[i]->nb[0] == EXPOSED) best = tri[i]->nd[1];
	    if (tri[i]->nb[1] == EXPOSED) best = tri[i]->nd[0];
	}

	/*get the gas node!*/
	best = node_mat(best, GAS);
	step = 0;
	besti = -1;
	reorder[best] = step++;

	while(step < nn) {
	    /*if this is a multiple node point, go to the other one*/
	    if ( num_nd(pt_nd(best)) == 2 ) {
			besti = best;
			if ( nd_pt(pt_nd(best),0) != best )
				best = nd_pt(pt_nd(best),0);
			else
				best = nd_pt(pt_nd(best),1);
			reorder[best] = step++;
	    }

	    /*walk the edges at this node*/
	    for(work = -1, i = 0; work == -1 && i < num_edge_nd(best); i++) {
			e = edge_nd(best, i);
			if ( (nd_edg(e,0) == best) ) {
				if ( nd_edg(e,1) != besti ) {
					work = besti = best;
					best = nd_edg(e,1);
					reorder[best] = step++;
				}
			}
			else {
				if ( nd_edg(e,0) != besti ) {
					work = besti = best;
					best = nd_edg(e,0);
					reorder[best] = step++;
				}
			}
	    }
	}
		
        break;


    case TWOD :

    for (nremain = nn, step = 0; step < nn; step++) {
		/* Find the node most wanting elimination */
		for (bestn = nn+1, i = 0; i < nremain; i++) {
			if ((nnb = nbrs[ remaining[ i]].v) < bestn) {
				bestn = nnb;
				besti = i;
				if (bestn <= Obestn) break;
			}
		}
		best = remaining[ besti];
		Obestn = bestn;
		bestl = &nbrs[ best];

		/* Take it off the remaining list and add it to the reorder list*/
		remaining[ besti] = remaining[ --nremain];
		reorder[ best] = step;

		/* Eliminate node best: take best out of each of its neighbors
		   adjacency lists, and union its adjacency list in instead */
		for (p = bestl->n; p; p = p->n) {
			work = p->v;
			if (work == best) continue;
			workl = &nbrs[ work];

			/* To speed union, generate membership mask */
			for (q = workl->n; q; q = q->n) mask[q->v] = 1;

			/* Take best out*/
			for (lastq = workl, q = lastq->n; q; lastq = q, q=q->n)
			if (q->v == best) { free_link(&lastq); break; }
			workl->v--;

			/* Form union by stuffing new elements in front */
			for (q = bestl->n; q; q = q->n) {
				if (!mask[q->v]) {
					new_link(&workl->n);
					workl->n->v = q->v;
					workl->v++;
				}
			}

			/* Reset mask */
			mask[best] = 0;
			for (q = workl->n; q; q = q->n) mask[q->v] = 0;

			/* Might have a new lower count [never happens?] */
			if (workl->v < Obestn) Obestn = workl->v;
		}

		/* Mark best as dead and gone */
		while( bestl->n) { free_link( &bestl); total++; }
    }

    /* Self protection */
    for (in = 0; in < nn; in++) mask[ reorder[in]]++;
    for (in = 0; in < nn; in++) assert(mask[in] == 1);
    free((char *)remaining);
    free((char *)mask);
    break;
    }

    if (talk)
    	printf("high water mark=%d [%f *asize]  loff=%d\n",
    			hwm, (float) hwm / ia[nn], total);
    
    free((char *)nbrs);
    free((char *)storage);
    free((char *)order);
    return( total - nn);
}

int o_compar(struct d_sort  *f1, struct d_sort *f2)
{
    if ( f1->x > f2->x ) return( 1 );
    else if ( f1->x < f2->x ) return( -1 );
    else if ( f1->mat > f2->mat ) return( 1 );
    else if ( f1->mat < f2->mat ) return( -1 );
    else return( 0 );
}

/**
 * Memory allocation routine for linked list kind of mooty.
 *
 * The main purpose of linked list is to avoid memory fragmentation which is
 * usually cased by allocating a huge chunk of dataset as an array. Since
 * linked list registers memory of next node in each node, we don't have to worry
 * about menance from huge chunk of memory allocation. Each node can be placed
 * sparsely within memory space without problem.
 *
 * This new_storage just allocates each node within a limited range (from *start to
 * *end.)
 */
/**
void new_storage()
{
    link_t *start, *end, *p;
    
    if (!storage) {
	storage = salloc( link_t, storage_size);
	start = storage;
	end   = storage + storage_size -1;
    }
    else {
	storage = sralloc( link_t, 2*storage_size, storage);
	start = storage + storage_size;
	end   = storage + 2*storage_size -1;
	storage_size *= 2;
    }
    assert( storage);
    
    for (p = start; p < end; p++) p->n = p+1;
    end->n = NULL;
    freelist = start;
}
**/

void new_storage()
{
    link_t *start;
    link_t *end;
    link_t *tmp;
    int i;

    // Make a new list
    if (!storage) {
		storage = (link_t*)malloc(sizeof(link_t));
		tmp = storage;
		for (i=0; i < storage_size-1; i++){
			tmp->n = (link_t*)malloc(sizeof(link_t));
			tmp = tmp->n;
		}
		// Last node --> set its next node pointer to NULL
		tmp->n = NULL;
		start = storage;
		end = tmp; // Useless, but well, I respect original authors, anyway.
    }
    // If we have an existing list
    else {
		// First, find the last node
		for (tmp = storage; tmp->n; tmp = tmp->n);
		// Now, we got tmp as the last node. Start populating the list
		for (i=0; i < storage_size-1; i++) {
			tmp->n = (link_t*)malloc(sizeof(link_t));
			tmp = tmp->n;
		}
		// last node
		tmp->n = NULL;
		end = tmp;
		start = storage;
    }
    assert(storage);

    freelist = start;
}

#ifdef notdef
void run_length(
     int nn,
     link_t *nbrs)
{
    int in, last, nrlen, rlen, sumrlen;
    float averlen;
    link_t *j;

    sumrlen = nrlen = 0;
    for (in = 0; in < nn; in++) {
	rlen = 1;
	for (j = nbrs[in].n->n; last = j->v, j->n; j = j->n) {
	    if (j->n->v == last-1)
		rlen++;
	    else {
		sumrlen += rlen;
		nrlen ++;
		rlen = 1;
	    }
	}
    }
    averlen = sumrlen / nrlen;
    printf("average run length: %f\n", averlen);
}


#endif
