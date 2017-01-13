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
/*   finel.h                Version 5.1     */
/*   Last Modification : 7/3/91 08:55:03 */

#ifndef FinelH
#define FinelH

/* definitions for functions in the c files of the finel directory */


void FEassmb(
    int *ia, int aoff,
    double *a,
    double *rhs);

#ifndef MELDOF
#define MELDOF 60
#endif

void addstf(
     int *ia, int aoff,
     double *a,
     double *rhs,
     int nel,
     double stiff[MELDOF*MELDOF],
     double wrhs[MELDOF],
     int eqns[MELDOF],
     int fixity[MELDOF],
     int dummy);

void ave_stress();

void localize(
     int ie,
     double *xl, double *disp,
     int *eqns, int *fixity,
     double *bc);

void dump_stiff(
     double *stiff, double *rhs, 
     int nel, int stride, int offset);

void FEsolve(
    int verbose);

double FErhsnorm(
		 double *a, double *rhs);

double FEupdnorm(
		 double *delta);

void FEsymb(
	    int **Pia, int **Pil, int *Paoff, int *Ploff, int **Ppv);

void FEsymb1(
	     int **Pia, int **Pil, int *Paoff, int *Ploff, int **Ppv);

void build_ia(
	      int **pia)			/* The list to generate */;

int FEconnect(
	       int me,
	       short *taken,
	       int *num);

void FEscramble(
		int *reorder);

int FlopCount(
	      int *il);

int FEupdate(
	     double *delta,
	     int loop);

int XchSol(
	   double *v,
	   int which);

void mxvl(
	  int nv,
	  int *ia,
	  int aoff,
	  double *a,
	  double *v,
	  double *av,
	  int *lvl);

int bubbleUp(
	     int **Preorder);

void Dump_ij(
	     int *ia);

int DispUpd();

int CountDummy();

void dump_A(
	    int *ia, int aoff, int *il, int loff);

void dump_A_convex();

void DoBandOrd(
    int *ia,
    double *a);

void FDBandOrd();

void tomcal(
    int *ia,
    int *reord,
    int *lvls);

void tri6_shape(
		double s, double t,		/* natural coordinates*/
		double *cord,	/* x,y coords of all nodes*/
		double *xsj,	/* transformation jacobian determinant */
		double *shp);	/* shp[i][2] = shape function, node i,
				   shp[i][0/1] = x,y derivative of shp[i][2]*/

void tri6_stiff(double *,
		double *,
		double *,
		double *,
		void (*coeff)(double *, int, double *, double *),
		int);

void tri6_stress(int,
		 void (*coeff)(double *, int, double *, double *),
		 double *,
		 double *,
		 double *,
		 double *,
		 double *,
		 double *);

void tri6_Snodal_stress(double *,
			double *,
			double *,
			double *,
			void (*coeff)(double *, int, double *, double *),
			int,
			double *);

void do_tri6_nodal_stress(double *,
			  double *,
			  double *,
			  double *,
			  void (*coeff)(double *, int, double *, double *),
			  int,
			  double *,
			  int);

void test_tri6();

void tri7_shape(
    double s, double t,		/* natural coordinates*/
    double cord[7][2],	/* x,y coords of all nodes*/
    double *xsj,	/* transformation jacobian determinant */
    double shp[7][3]);	/* shp[i][2] = shape function, node i,
			   shp[i][0/1] = x)y derivative of shp[i][2] */

void tri7_stiff(
	       double *,	/* The stiffness matrix */
	       double *,		/* Coordinates of nodes */
	       double *,		/* extra right hand side stuff */
	       double *,		/* Displacements of nodes */
	       void (*coeff)(double *, int, double *, double *),		/* A function to return material coefficients */
	       int );

void tri7_stress(
		 int mat,
		 void (*coeff)(double *, int, double *, double *),
		 double *,
		 double *,
		 double *,
		 double *,
		 double *,
		 double *);

void tri7_nodal_stress(
		       double *,
		       double *,
		       double *,
		       double *,
		       void (*coeff)(double *, int, double *, double *),
		       int,
		       double *);

void test_tri7();


void FEfdiff(int *ia, int aoff, double *a, int *il, int loff, double *l, 
	     double *rhs, double *solo, double *deln, double *delo, 
	     double *derr);

void FEstep(int *ia, int aoff, double *a, int *il, int loff, double *l, 
	    double *rhs, double *solo, double *deln, double *delo, 
	    double *derr, double lami, double lamf);

void FEmilne(int *ia, int aoff, double *a, int *il, int loff, 
	     double *l, double *rhs, double *solo, double *deln, 
	     double *delm, double *derr);

int FEnewton(int *ia, int aoff, double *a, int *il, int loff, 
	     double *l, double *rhs);


#ifdef __cplusplus
extern "C" 
#endif
void tomcal_(int *neq,
		   int *nnb,
		   int *ia,
		   int *reord,
		   int *adkstk,
		   int *lvls);
    

#endif
