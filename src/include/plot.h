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
/*   plot.h                Version 5.1     */
/*   Last Modification : 7/3/91 08:55:16 */

#ifndef PlotH
#define PlotH

#include "constant.h"
#include "data_types.h"

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

/*downward compatibility for a while*/
EXTERN char *label;
EXTERN char *title;
EXTERN int sel_log;
EXTERN int znn;
EXTERN float *z;

/*function for quick sort of floats*/
//extern bool compar();

/*following stuff is all used in the three dimensional code*/
/*definitions of the corners we are looking at*/
#define RIGHT 0x01
#define LEFT  0x02
#define UPPER 0x04
#define LOWER 0x08

/*This structure is used to hold the data for 1d extracter*/
typedef struct d_str {
    float x;
    float y;
    int mat;
} d_str;

/*type of one d plot to make*/
#define XSEC 1
#define YSEC 2
#define ZSEC 3
#define BND  4

/*plot mins and maxs*/
EXTERN float xmin, xmax;
EXTERN float ymin, ymax;

/*viewport mins and maxs*/
EXTERN float vxmin, vxmax, vymin, vymax;

/*is the y axis upside down??*/
EXTERN int yflip;

EXTERN int pl_debug;

#ifdef __cplusplus
extern "C"
#endif
void xgPlotInit( void );

#ifdef __cplusplus
extern "C"
#endif
void xgUpdate( int reset_axis );



#ifdef __cplusplus
extern "C"
#endif
void check_x( void );

void contour(char *par, par_str* param);

int do_1d(
int ptype,			/*type of one d cross section*/
float val,			/*the value in x or y*/
struct d_str data[],
int mat1, int mat2,			/* two sides of an interface */
int byarc)			/* whether to go by arclength or x */;

int d_compar(
struct d_str *f1, struct d_str *f2);

int intersect(
float a[MAXVRT][3], 		/*the coordinates of the triangle*/
float val,		/*the constant value being searched for*/
float x[2][3]);		/*the intersection points*/

void draw_lab(
char *par,
par_str* param);

void mat_edges(
int dir,
float value,
float ymin, float ymax);

void get_edge(
float mval[],		/*the returned intersection points*/
int mat[],		/*the returned materials crossed*/
int *mnum,		/*the number of materials crossed*/
int dir,		/*the direction along which to find the crossings*/
float value)		/*the value for the constant line*/;

void material(int line_bound);

void option(char *par, par_str* param);

void node_pl(int i);

void edge_pl(int i);

void reg_pl (int i);

void surf_pl(int color);

void xgdebug();

void plot_1d(char *par, par_str* param);

void CheckBound(float *lo, float *hi);

void plot_2d(char *par, par_str* param);

void draw_vornoi();

void draw_points(float delt);

void draw_stress(float vleng, float smax, int ccol, int tcol);

void draw_flow(float vleng, float vmax, int col);

void print_1d(char *par, par_str* param);

void sel_var(char *par, par_str* param);


#ifdef __cplusplus
extern "C"
#endif
void xgSetBounds(
		 double lowX, double lowY,		/* Lower left corner  */
		 double upX, double upY);

#ifdef __cplusplus
extern "C"
#endif
void xgLogAxis(int xlog, int ylog);

#ifdef __cplusplus
extern "C"
#endif
void xgAxisLabels(char *xl, char *yl, char *ti);

#ifdef __cplusplus
extern "C"
#endif
int xgNewSet();

#ifdef __cplusplus
extern "C"
#endif
int xgNewSetLT(
int line_spec);

#ifdef __cplusplus
extern "C"
#endif
void xgSetName(
char *name);

#ifdef __cplusplus
extern "C"
#endif
void xgSetValue(
double val);

#ifdef __cplusplus
extern "C"
#endif
void xgSetMark(
int mark);

#ifdef __cplusplus
extern "C"
#endif
void xgNewGroup();

#ifdef __cplusplus
extern "C"
#endif
void xgPoint(
double xval, double yval);

#ifdef __cplusplus
extern "C"
#endif
void xgLabel(
char *lab,
double x, double y);

#ifdef __cplusplus
extern "C"
#endif
void xgSetScale(
double x, double y);

#ifdef __cplusplus
extern "C"
#endif
void xgClear();

void xgUpdate(int reset_axis);

void gmove(double x, double y);

void gdraw(double x, double y);

#endif
