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
/*   implant.h                Version 5.1     */
/*   Last Modification : 7/3/91 08:55:07 */

#ifndef ImplantH
#define ImplantH

#include "data_types.h"

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

/* the structures for the rectangular mesh */
struct rect_nd {
    int mat;
    double conc;
    double dam;
};

struct rect_pt {
    int nn;
    struct rect_nd *nd;
};

struct surf_info {
    int nmat;
    int mat[MAXMAT];		/* materials */
    double top[MAXMAT];		/* effective depth for material */
    double bot[MAXMAT];		/* effective depth for material */
    double lft[MAXDIM];		/* the left and right edges */
    double rht[MAXDIM];		/* the left and right edges */
    struct surf_info *next;
};

/* define ions which are not also impurities */
#define BF2 	-1

/* is there a manual overide value set for the implant */
EXTERN int override;
EXTERN double Rp, delRp, Rgam, Rkurt;

/* the ignore distance separator */
#define ERR	1.0e-10

/* is the value within ERR of the coordinate */
#define WITHIN(A, B) ( ((A)-(B) < ERR) && ((B)-(A) < ERR) )

/* A good dx to use in integrating charge, etc. */
#define  PRS_DX 5.0e-4

/* a data structure to hold the various interesting constants */
struct imp_info {
    double maxz;	/* Max allowable input to Pearson func */
    double area;	/* Area for this Pearson curve */
    double lat[2];	/* The lateral standard deviation*/
    double vert[5];	/* Vertical information array*/
    double dam[8];	/* Damage characterization */
    };

/* lateral distribution */
#define LATSIG 0
#define LATAREA 1
    
/* Pearson distribution offsets */
#define	 PRS_A0    0
#define  PRS_B0    1
#define  PRS_B2    2
#define	 PRS_RP	   3		/* Projected range     */
#define	 PRS_PEAK  4		/* Value at the centre */

/* Gaussian distribution offsets */
#define	 GUS_RP    0
#define  GUS_SIG   1

/* the type of vertical distribution */
EXTERN int imp_model;
#define PEARS 1
#define GAUSS 2

/* damage parameters for silicon */
#define DAM_A1  0
#define DAM_A2  1
#define DAM_A3  2
#define DAM_NV	3
#define DAM_C1  4
#define DAM_C2  5
#define DAM_Z0  6
#define DAM_WT  7

/* undefine this if your machine doesn't like register vars (esp. with -O) */
#define  Y_SLICE  TRUE 		/* get_edge() direction, want a y-slice */
#define  X_VAL  0		/* cord[X_VAL] is an X value */
#define  Y_VAL  1		/*   .... similar idea .... */

/* various double functions */
extern double imp_latr();

#define IMP_LAT( A, B, C ) (0.5 * ( erf((B)/(A)) - erf((C)/(A)) ) )


void setdam(
int imp,
double energy,
double *damstat);

double dam_vert(
double depth,
double *damstat);

double dam_lat(
double pos, double left, double right,
struct imp_info *data);

int
fgtmom(
	int	elem, int mattr ,
	double	energy ,
	double	*rp, double *sigp, double *gam1, double *kurt, double *later );

void gtprs(
	double	 rp, double sigp, double gam1, double kurt ,
	struct imp_info	 *cvals)		/* Pearson-IV
constants */;

double imp_latr(
double pos,
struct imp_info *data);

double
prson(
	double	 x ,
	struct imp_info	 *pc)		/* Pearson-IV constants */;

double
qeqv(
double	 xst, double xen ,
double	 qtot ,		/* total possible dose */
double	 dx ,
struct imp_info	 *data );

int qpgauss(
double	 dx ,
struct imp_info	 *cvals)	/* Pearson-IV constants) area) max
Z*/;

void qpprs(
	double	 dx ,
	double maxd,
	struct imp_info	 *cvals)	/* Pearson-IV constants) area)
max Z*/;

int
setprs(
	int	 ion ,			/* implanting ion */
	int	 mat ,			/* matterial type */
	double	 energy ,		/* implantation energy */
	struct imp_info	 *cvals )	/* implant information */;

double imp_vert(
double	depth ,
struct imp_info	*dat);

double func(
double l[MAXDIM],
double pt[MAXDIM],
struct imp_info	*dat);

double simp_int(
double l[MAXDIM],
double a[MAXDIM],        
double b[MAXDIM],        
double eps,
struct imp_info *dat);

double evaluate(
double l[MAXDIM],  
double lft[MAXDIM],        
double rht[MAXDIM],        
struct imp_info *dat);

double
zeqv(
	double	 dose ,
	double	 qtot ,
	double	 dx ,
	double bt,
	struct imp_info	 *data );

void implant(
char *par,
par_str* param);

int imp_select(
par_str* param,
int *imp,
int *ion);

void do_implant(
int ion,	/*the impurity number*/
double angle,	/*the implant angle*/
double dose,	/*the dose of the implant*/
double energy,	/*the energy of the implant*/
int secondary,	/*calculate a damage or plusone profile*/
int sol,	/*storage location of implant*/
int is,		/*storage location of I defects*/
int vs)		/*storage location of V defects*/;

void make_surf(
struct surf_info **v,
double ang,
double lat);

void get_mat( 
struct surf_info *cur,
int angle);

void free_surf(
struct surf_info **v);


#endif
