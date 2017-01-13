#ifndef OxideH
#define OxideH

#include "FEgeom.h"
#include "data_types.h"

void FEoxbc();

void FEsilbc();

void FEbc(
    float temp,
    char *par,
    struct par_str* param);

void refl_disp();

void FEdirichlet(
    char *par,
    struct par_str* param);

void face_ache(
    FEelt_typ *ae,
    int j);

void gas_fixities();

void sil_face();

void SilOxLoad();

void clear_bc();

void FEconvert(
     int element,
     int dof);

void dummy_sil(
    int doSil)			/* tru => simulate silicon */;

void FE2s4(
    int SavWhat);

void FEfree();

void valencies();

void stash_noniC(
    int in,
    double where[]);

double Odiff_coeff(
    int m, int s,			/* material, species */
    float temp)			/* why sigh */;

void O2diff_coeff(
		  float temp,
		  double **nuevo,
		  double **equil,
		  double *noni,
		  double *idf,
		  double *vdf,
		  double *iprt,
		  double *vprt
		  );

void H2Odiff_coeff(
		   float temp,
		   double **nuevo,
		   double **equil,
		   double *noni,
		   double *idf,
		   double *vdf,
		   double *iprt,
		   double *vprt
		   );

void Oboundary (
    int s,			/*which species we have*/
    bound_str *bval);

float Oss (
    int s,		/* Which species it is */
    int mat);		/* What material we're in */

float Orr (float temp, int s, double *normal, double noni);
     
float Ovel (
    float temp,			/* Temperature */
    int s,			/* Which species */
    float conc,			/* Oxidant concentration */
    double *n,			/* XY normal */
    double noni)		/* electron excess */;

float LenEff(
    float conc, float ks)	/* ks turns out more convenient than B/A */;

double KsFermiDep(double noni, double temp);

void FE_oxbulk(
     double *matco,
     int mat,
     double *eps,
     double *sig);

void ksubs(
    double *rval,		/* List of return values */
    double *sig, double *n,		/* Stress tensor, local normal */
    double *extra);		/* Info stored by arrangement with FEconvert */

void ThermSig(
    float temp1, float temp2);

void AddIntSig();

void plane_strain(
    double E, double v, double *matco);

void sup4_ecoeff(
     double *matco,
     int mat,
     double *eps, double *sig);

void synch2(
     double arg, double *val, double *deriv);

void exp_l(
    double arg, double *f, double *dfda, double lambda);

void test_bulk(
     double ex, double ey, double ez, double dx, double dy, double dz);

double asinh(
     double arg);

void coeffox (
    char *par,
    struct par_str* param);

void get_array(
    struct par_str* param,			/* The mysterious param factor. */
    char *name,			/* The name of the parameter. */
    float **fla,			/* The array of floats. */
    int *nfla);			/* Input: how many required. Output: number used */

void parse_array(
    char *input,
    float **Pfla,
    int *nfla);

int mono_array(
    float *fla,
    int nfla,
    char *name);

void elast_growth(
    float temp,
    double dt);

void elem_stiff(
    int ie)	/* Triangle number */;

int get_elasconn(
     int ind,			/* which node */
     short *taken,		/* the list of connected nodes */
     int *num);		/* the number of neighbors found initially max. */

void erfc_growth (
    float temp,		/* Processing temperature */
    int ornt,		/* Orientation of substrate */
    int oxhow,		/* Dry or wet oxidation */
		  double dt);

void fixup_t1d (
    float cutback);

void erf1_growth (
		  float temp,		/* Processing temperature */
		  int ornt,		/* Orientation of substrate */
		  int oxhow,		/* Dry or wet oxidation */
		  double dt,
		  int which_one);          /* Which miserable model */

int c_mater(char *par, struct par_str* param);

int ChosenMater(char *par, struct par_str* param, int other);

int ChosenBC(char *par, struct par_str* param, int other);

void comp_mat(
float temp);

void set_crystal();

void SupToXtal(double *normal, double *vec3);

float DoOriDep(
    double *dir,		/* Crystal direction */
    float valOri[3])		/* value in three known directions */;

void oxgrow (
    float temp,		/* Processing temperature */
    int ornt,		/* Orientation of substrate */
    int oxhow,		/* Dry or wet oxidation */
     double *dt);	/* Time increment */

void point_vel();

void move_point(
     double dt);

void tot_areas(
double *oar, double *nar);

int ClockTri(
    int FixEm);

void oxide_vel (
    float temp,		/* Processing temperature */
    int ornt,		/* Orientation of substrate */
    int oxhow,		/* Dry or wet oxidation */
    double dt);

void Onvel(
    int io,			/* Oxide node number */
    float vel[2])		/* Returned velocity */;

int NeedNewFlow( );

void WipeVel();

void deal_grove (
    float temp, 		/* temperature */
    int   ornt,			/* substrate orientation */
    int oxytyp,			/* dry/wet oxidation */
    float *arate, float *brate) 	/* A and B coefficients */;

float ThinOxideCorr(
    double thick,		/* thickness-dependent model (Massoud) */
    double normal[2]);		/* The local normal vector */

void cldep (
    float temp,
    float *lcl, float *pcl);

void linlook(
    float val, float *fla,
    int n,
    int *ixl,			/* returned index */
    float *s)			/* returned interval position */;

void do_deal_grove (
     float temp,		/* temperature */
     int s)			/* species */;

void obfix(double *Cord, double *cord);

void triox_stiff(
     double *, /* The stiffness matrix */
     double *,	/* extra right hand side stuff */
     double *,		/* Coordinates of nodes */
     double *,	/* The current solution/displacement */
     void (*coeff)(double *, int, double *, double *), /* Fetches material data */
     int);			/* What material this triangle is */

void triox_bc(
     double *, /* The stiffness matrix */
     double *,	/* extra right hand side stuff */
     double *,		/* Coordinates of nodes */
     double *,	/* The current solution/displacement */
     void (*coeff)(double *, int, double *, double *),		/* Fetches material data */
     int,			/* What material this triangle is */
     int *,	/* The nodal boundary codes */
     double *,		/* Any nodal boundary float info */
     int,			/* because all this is still not enough */
     int);		/* a variable of unsurpassed ugliness*/

void triox_stress(
    int,
    void (*coeff)(double *, int, double *, double *),
    double *,
    double *,
    double *,
    double *,
    double *,
    double *);

void triox_nodal_stress(
    double *,
    double *,
    double *,
    double *,
    void (*coeff)(double *, int, double *, double *),
    int,
    double *);

void TrioxBCinit() ;

void oxload(
     double *,	/* The stiffness matrix */
     double *,		/* extra right hand side stuff */
     double *,		/* Coordinates of nodes */
     double *,		/* The current solution/displacement */
     void (*coeff)(double *, int, double *, double *),		/* Fetches material data */
     int,			/* What material this triangle is */
     int *,		/* The nodal boundary codes */
     double *,		/* Any nodal boundary float info */
     int,			/* because all this is still not enough */
     int);		/* a variable of unsurpassed ugliness*/

void test_triox();

void test_tbc(
    int ie,			/* Which triangle to work on */
    int iv,			/* Vertex of triangle to tweak */
    double vx, double vy);	/* Velocities to tweak */

void test_jacob(
    int *ia, int aoff,		/* matrix map */
    int in,			/* node to tweak */
    int iv,			/* dof to tweak */
    double v,			/* tweak value */
    int *il, int loff);

void test_jackass();

void test_stress(
    int n);

void test_pbc();

double ffcos(
    double ax, double ay, double bx, double by);

void vert_growth (
    float temp)		/* Processing temperature */;

void visco_growth(
    float temp,
    double dt);

void stress_analysis(
     char *par,
     struct par_str* param);

void FEmathset(
    int flow)			/* Doing flow or stress calc? */;


void O2boundary(bound_str *bval);

void H2Oboundary(bound_str *bval);


void FE2s4all();
void FE2s4ox();
void FE2s4sil();

void noop(
	  double *stiff, /* The stiffness matrix */
	  double *wrhs,       /* extra right hand side stuff */
	  double *xl,         /* Coordinates of nodes */
	  double *disp,       /* The current solution/displacement */
	  void (*coeff)(double *, int, double *, double *),           /* Fetches material data */
	  int mat,                   /* What material this triangle is */
	  int *fixity,        /* The nodal boundary codes */
	  double *bc,         /* Any nodal boundary float info */
	  int ie,                    /* because all this is still not enough */
	  int fixme);                 /* a variable of unsurpassed ugliness */


#endif
