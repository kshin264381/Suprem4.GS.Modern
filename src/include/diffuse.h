/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   diffuse.h                Version 5.1     */
/*   Last Modification : 7/3/91  08:55:02 */

/************************************************************************
 *									*
 *	diffuse.h - This file contains data needed for the matrix setup	*
 *  and solve of the diffusion problem.					*
 *									*
 ************************************************************************/
#ifndef DiffuseH
#define DiffuseH

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

#include "data_types.h"

/*set up a definition for the norm function*/
EXTERN double norm();

/*add a couple of solution vectors for the diffusion timestep algorithm*/
EXTERN double total;		/*the total time so far*/
EXTERN double *old[MAXIMP];	/*solution at last time step*/
EXTERN double *mid[MAXIMP];	/*solution at the middle of the time step leap*/
EXTERN double *new_sol[MAXIMP];	/*solution at latest time step*/
EXTERN double *rhs[MAXIMP];	/*answer vectors for trapazoidal rule*/
EXTERN double *newb[MAXIMP];	/*answer vectors at current time point*/
EXTERN double *oldb[MAXIMP];	/*answer vectors at last time point*/

/*active concentrations and derivitives*/
EXTERN double *newa[MAXIMP];		/*active portion of each impurity*/
EXTERN double *mida[MAXIMP];		/*active portion of each impurity*/
EXTERN double *olda[MAXIMP];		/*active portion of each impurity*/
EXTERN double *dact[MAXIMP][MAXIMP];	/*partial of net w/r each imp*/
EXTERN double *equil[MAXIMP];		/*equilibrium concentrations if appropriate*/
EXTERN double *dequ[MAXIMP];		/*partial of equilibrium concentrations if appropriate*/
EXTERN double *net;		/*net concentration at each point*/
EXTERN double *dnet[MAXIMP];		/*derivitive of the net concentration at each point*/
EXTERN double *noni;		/*n over ni at each point*/

/*truncation errors as a function of impurity*/
EXTERN double LTE[MAXIMP];
EXTERN double ABE[MAXIMP];


/*the type of timestep flag*/
#define TR  1
#define BDF 2
#define SS  3

struct call_str {
    int type;
    double old_del, new_del;
    float temp;
    double **old, **mid, **nuevo;
    double **olda, **mida, **newa;
    double *oco, *mco, *nco;
    int *sol;
    int nsol;
    int *elim;
    int *il;
    double *l;
    int loff;
    };

EXTERN struct call_str cs;


void Sbdiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/
		  );

void Sbboundary( bound_str *bval );

void Sbactive(
int simple,
int nn,
float temp,
double **chem,
double **act,
double **equil,
double *noni
	      );

void antimony( char *par, par_str* param );

void Asdiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/
		  );

void Asactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni
	      );

void Asboundary( bound_str *bval );

void arsenic( char *par, par_str* param );

void Bediff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Beactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni);

void Beboundary(
bound_str *bval);

void beryllium(
char *par,
par_str* param);

void Bdiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Bboundary(
bound_str *bval);

void Gaboundary(
bound_str *bval);

void Ptboundary(
int imp,
bound_str *bval);


void Bactive(
int simple,
int nn,
float temp,
double **chem,
double **act,
double **equil,
double *noni);

void Gaactive(
int simple,
int nn,
float temp,
double **chem,
double **act,
double **equil,
double *noni);

void Ptypeactive(
int imp,
int simple,
int nn,
float temp,
double **chem,
double **act,
double **equil,
double *noni);

void boron(
char *par,
par_str* param);

void Cdiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Cactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni);

void Cboundary(
bound_str *bval);

void carbon(
char *par,
par_str* param);

void Csdiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Csboundary(
bound_str *bval);

void cesium(
char *par,
par_str* param);

void Gdiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Gactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni);

void Gboundary(
bound_str *bval);

void generic(
char *par,
par_str* param);

void Gediff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Geactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni);

void Geboundary(
bound_str *bval);

void germanium(
char *par,
par_str* param);

void Audiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Auboundary(
bound_str *bval);

void Aucoupling(
float temp, 		/*the temperature*/
double *area,		/*the area of each node*/
double **znew,		/*all the concentrations*/
double **equil,		/*the equilibrium concentrations*/
double **dequ,		/*the nodal areas*/
double **rhs);		/*the current right hand side*/

void gold(
char *par,
par_str* param);

void Idiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Icoupling(
float temp,
double *area,		/*the nodal areas*/
double **znew,		/*all the concentrations*/
double **equil,		/*the equilibrium concentration*/
double **dequ,		/*the derivitive of equilibrium concentration*/
double **rhs		/*the current right hand side*/);


void Isink( 
float temp,
double *area,           /*the nodal areas*/
double **znew,           /*all the concentrations*/
double **equil,         /*the equilibrium concentration*/
double **dequ,          /*the derivitive of equilibrium concentration*/
double **rhs);           /*the current right hand side*/


void Iboundary(
bound_str *bval);

void Itime_val(
int is,
double *rhsnm,
struct call_str *cs);

void interstitial(
char *par,
par_str* param);

void comp_intparam(
float temp);

#ifdef HUANG
void Isink(
float temp,
double *area,           /*the nodal areas*/
double **znew,          /*all the concentrations*/
double **equil,         /*the equilibrium concentration*/
double **dequ,          /*the derivitive of equilibrium concentration*/
double **rhs            /*the current right hand side*/);
#endif

float form_eval(
char *expr,
float total,
float cord[2]);

void Mgdiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Mgactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni);

void Mgboundary(
bound_str *bval);

void magnesium(
char *par,
par_str* param);

void Pdiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Pboundary(
bound_str *bval);

void Pactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni);

void phosphorus(
char *par,
par_str* param);

void Sediff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt)		/*the partial of the vacancy diffusivity*/;

void Seactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni);

void Seboundary(
bound_str *bval);

void selenium(
char *par,
par_str* param);

void Sidiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt		/*the partial of the vacancy diffusivity*/);

void Siactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni);

void Siboundary(
bound_str *bval);

void silicon(
char *par,
par_str* param);

void Sndiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt);		/*the partial of the vacancy diffusivity*/

void Snactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni);

void Snboundary(
bound_str *bval);

void tin(
char *par,
par_str* param);

void trap(
char *par,
par_str* param);

void Vdiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt);		/*the partial of the vacancy diffusivity*/

void Vcoupling(
float temp, 		/*the temperature*/
double *area,		/*the area of each node*/
double **znew,		/*all the concentrations*/
double **equil,		/*the equilibrium concentrations*/
double **dequ,		/*the nodal areas*/
double **rhs);		/*the current right hand side*/


void Vsink( 
float temp,
double *area,           /*the nodal areas*/
double **znew,           /*all the concentrations*/
double **equil,         /*the equilibrium concentration*/
double **dequ,          /*the derivitive of equilibrium concentration*/
double **rhs);           /*the current right hand side*/


void Vboundary(
bound_str *bval);

void Vtime_val(
int vs,
double *rhsnm,
struct call_str *cs);

void vacancy(
	     char *par,
	     par_str* param);

void comp_vacparam( float temp );

void Zndiff_coeff(
float temp,		/*temperature*/
double **znew,		/*the current concentration set*/
double **equil,		/*the equilibrium concentration set*/
double *noni,		/*the scaled electron concentration*/
double *idf,		/*the interstitial diffusivity*/
double *vdf,		/*the vacancy diffusivity*/
double *iprt,		/*the partial of the interstitial diffusivity*/
double *vprt);		/*the partial of the vacancy diffusivity*/

void Znactive(
int simple,
int nn,
float temp,
double **conc,
double **act,
double **equil,
double *noni);

void Znboundary(
bound_str *bval);

void zinc(
char *par,
par_str* param);

void get_active(
int simple,
float temp,
double **chem,		/*the solution values to use in calculation*/
double *net,		/*the net active concentration*/
double **dnet,		/*partial of the active concentration*/
double **active,	/*the active substitutional concentrations*/
double **equil,		/*the equilibrium concentration*/
double **dequ);		/*the equilibrium concentration*/

void gaas_act(
float temp,
double *net)		/*the net active concentration*/;

void PSbblock_set(
int ss,			/*block number being solved*/
float temp,
int nsol,
int *solve,
double *cpl,			/*coupling areas*/
double **chem,			/*pointer to solutions at this timestep*/
double **sub,			/*pointer to solutions at this timestep*/
double *inter,			/*scaled interstitial conc*/
double *vacan);			/*scaled vacancy conc*/

void BAsblock_set(
int ss,			/*block number being solved*/
float temp,
int nsol,
int *solve,
double *cpl,			/*coupling areas*/
double **chem,			/*pointer to solutions at this timestep*/
double **sub,			/*pointer to solutions at this timestep*/
double *inter,			/*scaled interstitial conc*/
double *vacan);			/*scaled vacancy conc*/

void neut_block_set(
int sol,		/*block number being solved*/
float temp,		/*temperature*/
int nu1,		/*not used*/
int *nu2,		/*not used*/
double *area,		/*coupling coefficients*/
double **chem,		/*pointer to solutions at this timestep*/
double **nu3,		/*not used*/
double *nu4,		/*not used*/
double *nu5);		/*not used*/

void chrg_block_set(
int ss,			/*the storage location*/		 
int sol,		/*impurity solution number*/
double *df,		/*the diffusivity*/
double *dprt,		/*the diffusivity derivitive*/
double *val,		/*the value diffusing*/
double *lval,		/*the log of the value diffusing*/
double *sqval,		/*the sqrt of the value diffusing*/
double *dval[MAXIMP],	/*the derivitive of w/r to sol variables*/
double *pot,		/*the potential values*/
double spot);		/*the sign for the potential terms*/

void poisson_block(
int sol,			/*block number being solved*/
float temp,
int nsol,
int *solve,
double *area,			/*coupling area*/
double **chem,			/*pointer to solutions at this timestep*/
double **nu1,			/*not used*/
double *nu2,			/*not used*/
double *nu3);			/*not used*/

void init_pseudo(
double temp);

void get_defaults(
int imp);

void get_Cstar(
int imp,
int nv,
float temp,
double *noni,
double *equil,
double *dequ);

void IVblock_set(
int ss,			/*block number being solved*/
float temp,
int nsol,
int *solve,
double *area,			/*coupling area*/
double **chem,			/*pointer to solutions at this timestep*/
double **sub,			/*pointer to solutions at this timestep*/
double *inter,			/*scaled interstitial conc*/
double *vacan);			/*scaled vacancy conc*/

void defect_block(
int sol,			/*block number being solved*/
float temp,
double **chem,
double *inter, double *vacan,
double *area);

void diffuse_init();

void diffuse( char *par, par_str* param);

void make_movie(
char *commands,		/*the commands*/
float time);

void moving_bound(
double dt,			
double scale);

double norm(
double *arr,
int n);

int pl_interface(int a, int *b, int *c);

void poly_gs(int i, float temp, double time) ;

void poly_init(int oldnn, double temp, double pressure, double thick, int imp,
	       double conc, int ndvi) ;

void grain_gs(int i, float temp, double lastddf) ;

void predep( );

void init_diffuse();

int upd_diff(
double delta,
double *area,
int doit,
int min_ok);

void upd_matrix();

void diff_alloc( );

void ia_calc( );

void diff_blocks();

void arr_alloc(
int *no_compute);

void assem_map( );

void do_setup(
double *rhs2)		/*the two norm of the right hand side*/;

void zero_setup(
struct call_str *cs);

void bval_compute(
float temp,
double **chem,
double del);

void time_val(
int sol,
double *rhsnm,
struct call_str *cs);

void setup_2d(
int sol,		/*the solution number to setup*/
float temp,		/*the temperature*/
int nsol,
int *solve,
double *inter,		/*the interstitial concentrations*/
double *vacan,		/*the vacancy concentrations*/
double **chem,		/*the current concentrations*/
double **act,		/*the current concentrations*/
double **equil,		/*the current equilibrium concentrations*/
double *newarea,	/*the area of each node*/
bound_str *bval,
int bcnum);

void init_rhs(
float temp,
double **old,
double **olda,
double *oarea);

int soldif_tr(
double del_t,		/*the amount of time step*/
float temp,		/*the temperature of the solve*/
double **old,		/*the answer of the previous timestep*/
double **nuevo,		/*the answer of this solution*/
double **olda,		/*the answer of the previous timestep*/
double **newa,		/*the answer of this solution*/
double *oldarea,	/*the area of the previous timestep*/
double *newarea,	/*the area of this solution*/
int init);		/*whether or not to reuse the last factorization*/

int soldif_bdf(
double del_t,		/*the amount of time step*/
double old_t,		/*last time step*/
float temp,		/*the temperature of the solve*/
double **nuevo,		/*the answer of this solution*/
double **mid,		/*the answer of the previous timestep*/
double **old,		/*the answer of the really old timestep*/
double **newa,		/*the answer of this solution*/
double **mida,		/*the answer of the previous timestep*/
double **olda,		/*the answer of the really old timestep*/
double *newarea,	/*the area of this solution*/
double *midarea,	/*the area of the previous timestep*/
double *oldarea,	/*the area of the really old timestep*/
int init);		/*whether or not a setup has been done*/

void steady_state(
float temp,
int nsol,
int *sol,
double **nuevo,
double *newarea);

int soldif( 
	   int init,
	   char *label,
	   void (* do_setup)(double *));

int update_vars(
double *nm,
int nsol,
int *sol,
int nv,
double **vars,
double **update);

void solve_time(
float time,		/*the length of time to take this diffusion/oxidation*/
float temp,		/*the temperature at which this is performed*/
double itime,
int timemeth, 		/*time step method*/
int dump,	/*frequency of file writes*/
char *movie,		/*the movie command string*/
int cont);

void do_oxide(float temp,
	      int ornt,
	      int oxhow,
	      double *dt);

double do_disloop(float temp,
	      double *dt);

double solstep(
double del_df,
float temp,
int timemeth,
int compute);	/*flag for computation of the split variable*/

void dump_data(
double total,
int dump,
char *movie,
int *cnt);

float time_form(
    char *expr,
    float total,
    float dt);

double start_step(
float temp,
int cont);

double grid_dt();

void int_dose();

void clear_row(
int n,
int sol);

double timed_trbdf(
double delta,
float temp,
int compute);	/*flag for computing the comparison*/

void do_vel(
double delta,
double gam);


#endif
