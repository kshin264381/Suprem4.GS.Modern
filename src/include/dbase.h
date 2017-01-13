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
/*   dbase.h                Version 5.1     */
/*   Last Modification : 7/3/91  %U */

#ifndef DbaseH
#define DbaseH

#include "data_types.h"
#include "geom_data_types.h"

// extern struct edg_str;
// extern struct tri_str;
// extern struct list_str;
// extern struct nd_str;
// extern struct pt_str;
// extern struct reg_str;

//struct edg_str;
//struct tri_str;
//struct list_str;
//struct nd_str;
//struct pt_str;
//struct reg_str;

/* forward declarations for all files in the dbase directory 
   except locate.c which has locate.h for this purpose       */


void add_impurity(
		  int imp,	
		  float background,
		  int mat);

void mtest1(
	    char *when);

void mtest2(
	    char *when);

void clock_tri();

void sclk_tri(
	      int ie);

void sum_coeff(
	       double *area);

void dose_calc();

void set_dim(
	     int dim);

void dis_all();

void waste();

void lose_impurity( int imp);
     
int MeshValid();

void MeshInvalidate();

int InvalidMeshCheck();

int mk_edg(
	   int n1, int n2);

void pr_edg(
	    int e);

void dis_edg();

void dis_1edg (
	       struct edg_str **e);

void build_edg();

char *alloc_tri();

int mk_ele(
	   int nv, int *v,
	   int nb, int *b,
	   int r,
	   int flag);

int mk_ele_pt(
	      int nv, int *v,
	      int reg);

int mk_ele_nd(
	      int nv, int *v,
	      int r);
     
void dis_tri();

void dis_1tri (
	       struct tri_str **t);

void make_bc();

void tri_to_node();

void nxtel();

int flip();

void tri_flip(
	      int it, int tp, int tq);

int ck_flip(
	    int we);

double good_tri(
		int i, int j, int k);

char * ccentre (
		int i, int j, int k,		/* Indices of points. */
		float *c,		/* Centre. */
		double       *r);    	/* Radius. */

void d_perp(
	    struct edg_str *ep,
	    int n,
	    float alph[MAXDIM]);

double l_edge(
	      int ie);

double intang(
	      int n1, int n2, int n3);

int lil(
	float *p, float *dp, float *q, float *dq, float *alph);

void geom(
	  char *when);

void sum_edge();

float gimme_ehed(
		 int it);

float ehed_tri(
	       int t, int e);

void do_geom(
	     int t);

void tri_geom(
	      int ie);

void seg_geom(int ie);

void repair_obtuse ();
     
void chp_fix(
	     int obe);

int DetectLoop();

void drawtri(
	     int ie);

void drawpt(
       int ip,
       char *lab);

int grid_add(			/* Returns fix_conn */
	     double dt);

void back_pedal(
		double *dt);

void grid_sub(
	      double dt);

void ChooseKillNodes(
		     double dt);

int CknMcLen(
	     double dt,			/* time step */
	     double sepTol, double obTol,
	     double abstol);
     
int CknMcObt(
	     double dt,			/* time step */
	     double sepTol, double obTol);


float BadObtuse(
		float x, float y, float x1, float y1, float x2, float y2);

void kill_node(
	       int in);

void trotate(
	     int ie,			/* The triangle */
	     int iv,			/* The index of the boundary node */
	     int clock,			/* 1 for clockwise, 0 for cc */
	     int *Pje,			/* The outside neighbor (loop-breaker) */
	     int *Poje,			/* The inside neighbor */
	     int *Pj,			/* index of rotation node in oje */
	     int *Pjj);			/* index of leading node or trailing
						   triangle */
     
void add_list(
	      struct list_str *l,
	      int item);

void dis_list(
	      struct list_str *l);

void pr_list(
	     struct list_str *l);

void sub_list(
	      struct list_str *l,
	      int item);

void sub_nd_pt(
	       int n, int p);

int twhich (int in, int wanted);

int nwhich (
	    int in, int wanted);

int ewhich(
	   int it, int wanted);

int node_mat (
	      int in, int wanted);

void create_db(
	       int mk_nodes);

void bd_connect(
		char *when);

int num_neigh_nd(
		 int n);

int neigh_nd(
	     int n, int i);

int num_neigh_pt(
		 int p);

int neigh_pt(
	     int p, int i);

void elim_dups(
	       int *taken,
	       int *num);

void NativeOxide(
		 double thick);
     
int new_layer(
	      int Msil, int Mox,	/* Layer below, above interface */
	      double oxide_grid,
	      double dt);
     
int SplitNode(
	      int aNode,		/* Point being split as seen by mOx */
	      float *v,			/* Displacement for new point */
	      int mSil);

int exposed(
	    int in);
     
int split_thick(
		int n,
		int Mox, int Msil,
		float vdisp[MAXDIM]);

float rough_perp(
		 int p,		/* Point */
		 int matl,		/* + material = node */
		 float dir[MAXDIM]);

int local_normal (
		  int n,		/* Node we care about */
		  int matl,		/* Facing material, can be GAS */
		  float ln[MAXDIM]);

int dlocal_normal (int n, int matl, double dln[2]);

void test_ln(int i, int m);

char *alloc_nd();

int mk_nd(int p, int m);

void dis_nd();

void dis_1nd (struct nd_str **n);

void make_nodes();

void node_to_tri();

char *alloc_pt();

int mk_pt(int nc, float *cord);

void dis_pt();

void dis_1pt (struct pt_str **p);

void pt_to_node();

int mk_reg(int mat);

void dis_reg();

void dis_1reg (struct reg_str **r);

void build_reg(int r);

void rem_1nd(int n);

void rem_ele(int t, int expos);

void rem_reg(int r);
     
void rem_edg(int ie);

void rem_edg_end(int ie, int nr);

void rem_pt(int ip);


void check_active();

int ig2_read (
    char *name,		/* where the data is hiding */
    int flip,		/* flip y coords? */
    float scale)	/* scale factor */;

void ugh(int line,
	 char *s,
	 int ierr);

int ig2_write (
    char *name,
    int lflip,
    float scale);

void initialize(
char *par,
par_str* param);

int newbackside(
float line);

int pi_write(
     char *name,		/* Where to write it */
     int show);

char *pi_elect(
     int *nb, int *nelect, int  **nbc, int **ietype);

void  ShowElect(
     int show, int  Pnelect, int  Pnb, int  *Pnbc, int  *Pietype);

char *pi_dop(
     float **r1, float **tconc);

char *pi_mat(
     int *Pnp, int  *Pne, int *Pnmat, int  **Ppt, int  *Preg);

void reflect_grid(
     float sign,		/* -1 is left, +1 is right */
    int xy);			/* 0 is x 1 is y */

void profile(char *par, par_str* param);


int read_data( float *val, char *infile, int col, float offset  );


void rect_boot();

void rline (
    char *par,
    par_str* param);

int smooth (
    int dim);

int addint (
    int dim);

void dvpram (
             double hl, double hr, double el, int *nnew, 
	     double *ratio, double *first);

int squares(
    float new_mr);

void rregion(
    char *par,
    par_str* param);

void redge(
	   char *par,
	   par_str* param);


int tag_look(
    par_str* param,		/* Magic cookie for the parser */
    char *name,		/* Name of desired parameter */
    int dim);

int simpl_write(
char *SIMPLfilename,
		char *SIMPLheaderfilename);

void write_SIMPLPolygon(
FILE *SIMPLfile);

void write_SIMPLGrid(
FILE *SIMPLfile,
float *xloc,
int xlocsize,
float *yloc,
int ylocsize);

void structure(
char *par,
par_str* param);

void newcat(
char **a,
char *b);

int skel_write( 
     char *str, int i1, float f1);

int polygon_etch( 
char *str, int foo) ;

int pif_write( 
	      char *str);

#endif
