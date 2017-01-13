/*************************************************************************
 *									 *
 *     Copyright c 1989 The board of regents of the University of 	 *
 *                      Florida.  All rights reserved.			 *
 *									 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   device.h                Version 5.1     */
/*   Last Modification : 7/3/91  10:38:29 */

/************************************************************************
 *									*
 *	This file contains definitions of useful things for the device	*
 *  solver.  								*
 *									*
 ************************************************************************/

#ifndef DeviceH
#define DeviceH

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

#define MAXCKT 3000	/*the maximum number of circuit nodes*/

#define IS_SEMI(A) ( ((A) == Poly) || ((A) == Si) || ((A) == GaAs) )
#define EX BC_OFFSET+2
#define BC BC_OFFSET+1
#define NBR(I,J) (tri[(I)]->nb[(J)])

/*a contact is a semiconductor material touching an Exposed or backside or anything
  touching aluminum*/
#define IS_CONT(A,B) ( (IS_SEMI(mat_reg(reg_tri((A)))) && \
		       ( (NBR(A,B) == EX) || (NBR(A,B) == BC) ) ) || \
		       ( (NBR(A,B) > 0) && (mat_reg(reg_tri(NBR(A,B))) == Al ) ) )

/*Contact information*/
struct contact_str {
    char *name;		/*name of the contact*/
    int np;		/*the number of points*/
    int *pt;		/*list of the points involved*/
    int *ndc;		/*list of the contact material nodes involved*/
    int curnod;		/*circuit node for current out this contact*/
    int volnod;		/*circuit node for voltage at this boundary*/
    double conres;	/*the contact resistance in ohm/cm2*/
};

#include "data_types.h"

/*ten is the most external contacts allowed*/
EXTERN struct contact_str contacts[10];
EXTERN int n_con;			/*number of contacts*/
EXTERN int *nd2cont;
EXTERN int *pt2cont;

/*the total net dopant*/
EXTERN int dopcalcdone;

/*the device width*/
EXTERN double dev_width;
EXTERN double qfn, qfp;	 /*fixed quasis for sub set solutions*/
EXTERN double devVt, devVti;		

EXTERN int n_ckt;	/*the number of circuit nodes*/

/*various branch relationships*/
struct vss_str {	/*voltage supply*/
    int con; 		/*the contact it is attached to*/
    double val;		/*the voltage value of v(con)*/
};
EXTERN int n_vss;
EXTERN struct vss_str vss[MAXCKT];

/* These are already defined in matrix.h */
/*EXTERN int *dev_il;	column index pointers for steady state solve*/
/*EXTERN int dev_loff;	upper triangular offset for steady state solve*/
/*EXTERN double *dev_l;	the lower triangular values for steady state*/
/*EXTERN int dev_sol[MAXIMP];	solution numbers of those being worked on*/
/*EXTERN int dev_nsol;		number of those being worked on*/

EXTERN int dev_elim[MAXIMP];		/*eliminate stuff*/

/*some circuit connectivity pieces*/
EXTERN int *ia_cc, ao_cc;
EXTERN int *ia_cn, ao_cn;

/*band edges as a function of doping and material*/
EXTERN double *Ec, *Ev;

void Nmobil(float a, double **b, double **c, double *d, double *e,
	    double *f, double *g, double *h);

void Ncoupling(
float temp,
double *area,		/*the nodal areas*/
double **neuvo,		/*all the concentrations*/
double **equil,		/*the equilibrium concentration*/
double **dequ,		/*the derivitive of equilibrium concentration*/
double **rhs		/*the current right hand side*/);

void Nboundary(
bound_str *bval);

double Phi_n(
double elec, double psi, double ec,
int mat,
double *dqdp, double *dqde);

void electron(
char *par,
par_str* param);

void Hmobil(float a, double **b, double **c, double *d, double *e,
            double *f, double *g, double *h);

void Hcoupling(
float temp,
double *area,		/*the nodal areas*/
double **nuevo,		/*all the concentrations*/
double **equil,		/*the equilibrium concentration*/
double **dequ,		/*the derivitive of equilibrium concentration*/
double **rhs		/*the current right hand side*/);

void Hboundary(
bound_str *bval);

double Phi_p(
double hole, double psi, double ev,
int mat,
double *dqdp, double *dqdh);

void hole(
char *par,
par_str* param);

void Psiboundary(
bound_str *bval);

void psi_card(
char *par,
par_str* param);

void circuit_setup(
int ss,
float temp,
int nsol,
int *solve,
double *coeff,
double **val,
double **nu1, double *nu2, double *nu3);

void contact(
char *par,
par_str* param);

void gen_contact();

void find_con(
int n,		/*the node to check*/
int nc,		/*the contact number we're working with*/
int *chk[3],	/*the list of checked nodes*/
int *len) /*the allocated length of nodes*/;

void poisson_block(
int sol,		/*block number being solved*/
float temp,
int nsol,		/*number of solutions being worked on*/
int *solve,		/*the solution values*/
double *coeff,		/*coupling coefficients*/
double **chem,		/*pointer to solutions at this timestep*/
double **nu1,		/*not used*/
double *nu2,		/*not used*/
double *nu3);

void hole_block(
int sol,			/*block number being solved*/
float nu1,			/*not used*/
int nu2,			/*not used*/
int *nu3,			/*not used*/
double *coeff,			/*coupling coefficients*/
double **chem,			/*pointer to solutions at this timestep*/
double **nu4, double *nu5, double *nu6);

void electron_block(
int sol,			/*block number being solved*/
float nu1,			/*not used*/
int nu2,			/*not used*/
int *nu3,			/*not used*/
double *coeff,			/*coupling coefficients*/
double **chem,			/*pointer to solutions at this timestep*/
double **nu4, double *nu5, double *nu6);

void GSD_block_set(
int nn,			/*the number of nodes*/
double *m,		/*the mobility transport term*/
double *u, double *eu,		/*the u and exp(u) variables*/
double *v,		/*the variable to take the gradient*/
double *cur,		/*the fluxes*/
double *coeff,		/*coupling coefficients*/
double *djdm,		/*the derivative of the fluxes w/r to m*/
double *djdu,		/*the derivative of the fluxes w/r to eu*/
double *djdv);		/*the derivative of the fluxes w/r to v*/

void dev_prep(
double *area);

void dev_net();

int ckt_scram(int *reorder);

void ckt_ia_bld( );

void dev_solve(double *area, char *movie);

void dev_setup(double *rhs2);		/*the two norm of the right hand side*/

void device_init();

void device(char *par, par_str* param);

void init_dev(double temp);


#endif
