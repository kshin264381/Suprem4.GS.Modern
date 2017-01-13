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
/*   refine.h                Version 5.1     */
/*   Last Modification : 7/3/91 08:55:19 */

#ifndef RefineH
#define RefineH

#include <stdio.h>
#include "data_types.h"

/************************************************************************
 *									*
 *	refine.h - This include holds the globals for the refinement	*
 *  code.								*
 *									*
 ************************************************************************/

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

// extern struct LLedge;
// extern struct sreg_str;

struct LLedge;
struct sreg_str;

struct line {
    double x, y;	/*the location of the point*/
    int p;		/*the point index*/
    int map;		/*the match on the other line*/
    };

#define XC(A) pt[A]->cord[0]
#define YC(A) pt[A]->cord[1]

/*the condition of points in the etch*/
#define OUT 0
#define IN 1
#define ON 2

/* forward declarations for functions in the refine directory */

void div_edg(
int ie);

void dvedge (
int ie,			/* Edge to divide. */
double lej,		/* Length of edge. */
int nnew,		/* Number of new nodes to add */
double ratio,		/* Ratio of each interval to next. */
double first)		/* First interval as a fraction of lej. */;

int cr_sreg (
int mat)	/* Material number of region. */;

int cr_tri (
int rn,		/* Region it belongs to. */
int ir)		/*skeleton region we are using*/;

void ad_edge (
   int ir,		/* Region to add to. */
   int ie,		/* Edge to add. */
   struct LLedge *lep,	/* Edge to enter around */
   int iscc, 		/* Does  this edge conform with region order? */
   int pos)		/* Whether before or after lep. */;

struct LLedge * eindex (
    int ir,	/* Region to search */
    int ie)	/* Edge to find. */;

int add_ang (
    struct sreg_str *r,	/* Region to update. */
    struct LLedge *lep) /* New edge to add. */;

void trip_tri(
int p,
int *t1, int *m1, int *t2, int *m2);

void triple_fix(
int p,		/*the triple point*/
int p2)		/*the other point*/;

int de_loop(
int *nop,
struct line *op,
double eps);

void user_deposit(
char *par,
par_str* param);

int build_skel(
int nump,
struct line *p,
int numop,
struct line *op,
int div,
int rn,
double sp,
int square);

void user_etch(
    char *par,
    par_str* param);

void etch(
float *p[MAXDIM+1],
int nump,
int mater);

void grid(
int ir,
int rbn,
int bflag,
int spflag);

void lineseg(
int ir,
int rbn);

int hinit(
int ir,
double sp);

void interp(
int r1, int r2);

void interp_1d(
int in, int ie2);

void interp_2d(
int in, int ie2);

void fac3x3(
double a[3][3],
int pvt[3]);

void sol3x3(
double a[3][3],
int pvt[3],
double ans[3]);

int gen_offset(
int nump,		/*the number of points in p*/
struct line *p,		/*the line*/
struct line **pop,	/*the offset*/
double thick,		/*size of the offset*/
double space)		/*size of the spacing*/;

void add_toline(
int *nop,
struct line *op,
double x, double y,
int m,
double err);

void del_fromline(
int index,
int *nop,
struct line *op);

void phys_etch(
double *rate,		/*the etch rate as a function of material*/
double time);

void compute_rate(
double *rate,	/*etch rate of each material*/
int *prio)	/*the material priority*/;

void rate_trip(
int p,
double rate,		/*the etch rate as a function of material*/
int t1, int t2);

int rect_div(
struct sreg_str *r,
struct LLedge **lep1, struct LLedge **lep2);

int dvrecedg(
struct sreg_str *r1, struct sreg_str *r2,
int ie,
struct LLedge *t1);

int rect_cnt(
struct sreg_str *r,
float *p,
int ie);

int chk_pt(float *q,
	   struct sreg_str *r);

void rect_tri(
int r, int rbn);

int skel_reg(
int r);

int sub_skel(
int s1, int s2,
int *s);

int sub_1dskel(
int s1, int s2,
int *s,
int *inside);

int sub_2dskel(
int s1, int s2,
int *s,
int *inside);

void check_in(
int s1, int s2,
int *inside,
int *ai, int *ao);

int on_bound(
int p,
int sr)			/*The coordinates of the points. */;

void plot_on(
int sr,
int *inside);

int pt_in_skel(
float c[3],
int sr)			/*The coordinates of the points. */;

struct LLedge *edg_crs(
int e,
int s2,
float c[MAXDIM]);

int sp_edge(
int ie,		/* Edge to split */
float c[MAXDIM],	/*place to split*/
int *ip,		/* New point*/
int mv);

int sp_gredg(
int ie,		/* Edge to split */
float c[MAXDIM],	/*place to split*/
int *p)		/* New point*/;

int upd_edg(
int ie,
int in);

void move_edge(
int ie, int ind,
float *c);

int insert_pt_edg(
int e,		/* Edge to split */
float c[MAXDIM],	/*place to split*/
int *p)		/* New point*/;

void new_edg_skel(
int ie,		/* Original edge*/
int enew,	/* new edge*/
int ir)		/* Skeleton */;

void new_edg_tri(
int e,		/* Edge to split */
int enew,
int it);

void string_etch(
char *infile,
int material);

int string_offset(
char *infile,
struct line **poffset_array);

int find_surf(
struct line **pp)		/*the point numbers*/;

void triang(
int ir,
int rbn,
int bflag,
int spflag);

int ck_clock(
int ir,
int fl);

//int acomp (
//    struct LLedge **lep1, struct LLedge **lep2);
int acomp(const void *zo, const void *zt);

void free_skel(
     int nr);

int sp_reg (
	struct LLedge *lep1, struct LLedge *lep2,	/* Edges to link */
	struct LLedge *bp_look);

int dupl(
	 int ir);

struct LLedge * chop (
    struct sreg_str *r,		/* Region to split. */
    int must)			/* Is some split necessary? */;

int divide(
struct sreg_str *r,
struct LLedge **lep1, struct LLedge **lep2);

double sk_cross(
		struct sreg_str *r,
		struct LLedge *lp1, struct LLedge *lp2,
		int rect);

struct LLedge * quadsplit(
			  struct sreg_str *rg);

double dmin (double a, double b);

double dmax (double a, double b);

double dabs (double a);

#ifdef _cplusplus
extern "C"
#endif
void speedie(FILE *f1, FILE *f2, FILE *f3);

#endif
