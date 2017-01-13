/*   gpcover.c                Version 1.9     */
/*   Last Modification : 8/29/94 10:45:56        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <strings.h>

#include "gplot.h"

/*flags for a log axis..*/
int xl, yl;

/*label flags*/
char *xlab, *ylab, *tlab;

/*next point is a move*/
int mv;

float xscl, yscl;
float xxmax, xxmin, yymax, yymin;
int axis_done;
int cur_line;
int line_type;

void axis_2d(
float xxmin, float xxmax,	/*limits for the x axis*/
float yymin, float yymax,	/*limits for the y axis*/
float xscale, float yscale,   /*scaling to be applied to the labels*/
float pxmax, float pymax,	/*physical screen size*/
int flip,		/*flip the y axis upside down*/
char *xlab, char *ylab,	/*axis labels*/
int ylog);		/*draw y axis with a log scale*/

extern "C" int gpmisc(...);
extern "C" int gplot2(...);
extern "C" int symbl2(...);
extern "C" int axtcs2(...);
extern "C" int symb2(...);
extern "C" int axplt2(...);

#ifdef __cplusplus
extern "C"
#endif
void xgPlotInit()
{
    axis_done = 0;
    cur_line = 1;
    line_type = 1;

    return;
}

#ifdef __cplusplus
extern "C"
#endif
void check_x()
{
 return;
}


#ifdef __cplusplus
extern "C"
#endif
void xgSetBounds(double lowX, double upX, double lowY, double upY)	
{
    xxmin = lowX;
    xxmax = upX;
    yymin = lowY;
    yymax = upY;
    return;
}


#ifdef __cplusplus
extern "C"
#endif
void xgLogAxis(
int xlog, int ylog)
{
    xl = xlog;
    yl = ylog;
    return;
}

#define STRDUP(str)       (char *)(strcpy(malloc((unsigned) (strlen(str)+1)), (str)))

#ifdef __cplusplus
extern "C"
#endif
void xgAxisLabels(
char *xl, char *yl, char *ti)
{
    if ( xlab != NULL ) {free(xlab); xlab = NULL;}
    if ( ylab != NULL ) {free(ylab); ylab = NULL;}
    if ( tlab != NULL ) {free(tlab); tlab = NULL;}

    if (xl != NULL) xlab = STRDUP(xl);
    if (yl != NULL) ylab = STRDUP(yl);
    if (ti != NULL) tlab = STRDUP(ti);
    return;
}

#ifdef __cplusplus
extern "C"
#endif
int xgNewSet()
{
    if (!axis_done) {
	int gp_iv[4]; float gp_fv[4];
	float pxmax, pymax, tmp;

	gpgeti(G_PSIZE, gp_iv, gp_fv);
	pxmax = gp_fv[0]; pymax = gp_fv[1];

	gplot2(G_RESET, G_TRANS, 0.0, 0.0);
	gplot2(G_RESET, G_SCALE, 0.0, 0.0);
	gplot2(G_RESET, G_ROTATE, 0.0, 0.0);

	if (yscl < 0) {
	    tmp = yymax;
	    yymax = -yymin;
	    yymin = -tmp;
	}

	axis_2d(xxmin, xxmax, yymin, yymax, xscl, yscl, pxmax, pymax,
		(yscl < 0.0), xlab, ylab, yl);

	axis_done = 1;
    }
    gnline(cur_line);
    cur_line++;
    mv = 1;

    /* set clipping boundaries */
    gplot2( G_CLIPL, G_ONLOG, xxmin, yymin );
    gplot2( G_CLIPH, G_ONLOG, xxmax, yymax );

    return(0);
}

#ifdef __cplusplus
extern "C"
#endif
int xgNewSetLT(
int line_spec)
{
    if (!axis_done) {
	int gp_iv[4]; float gp_fv[4];
	float pxmax, pymax, tmp;

	gpgeti(G_PSIZE, gp_iv, gp_fv);
	pxmax = gp_fv[0]; pymax = gp_fv[1];

	gplot2(G_RESET, G_TRANS, 0.0, 0.0);
	gplot2(G_RESET, G_SCALE, 0.0, 0.0);
	gplot2(G_RESET, G_ROTATE, 0.0, 0.0);

	if (yscl < 0) {
	    tmp = yymax;
	    yymax = -yymin;
	    yymin = -tmp;
	}

	axis_2d(xxmin, xxmax, yymin, yymax, xscl, yscl, pxmax, pymax,
		(yscl < 0.0), xlab, ylab, yl);

	axis_done = 1;
    }
    line_type=line_spec;
    gnline(line_type);
    cur_line++;
    mv = 1;

    /* set clipping boundaries */
    gplot2( G_CLIPL, G_ONLOG, xxmin, (yscl>0) ? yymin : -yymin );
    gplot2( G_CLIPH, G_ONLOG, xxmax, (yscl>0) ? yymax : -yymax );

    return(0);
}


#ifdef __cplusplus
extern "C"
#endif
void xgSetName(
char *name)			/* New set name */
{
 return;
}

#ifdef __cplusplus
extern "C"
#endif
void xgSetValue(
double val)
{
 return;
}


#ifdef __cplusplus
extern "C"
#endif
void xgSetMark(
int mark)
{
 return;
}


#ifdef __cplusplus
extern "C"
#endif
void xgNewGroup()
/*
 * Set up for reading new group of points within a dataset.
 */
{
    mv = 1;
    return;
}

#define INITSIZE 4
#ifdef __cplusplus
extern "C"
#endif
void xgPoint(
double xval, double yval)		/* New point         */
{
    if (mv)
	gplot2(G_MOVE, 0, xscl*xval, yscl*yval );
    else
	gplot2(G_DRAW, 0,  xscl*xval, yscl*yval );
    mv = 0;

    return;
}


#ifdef __cplusplus
extern "C"
#endif
void xgLabel(
char *lab,
double x, double y)
{
    float hsize, wsize;

    x *= xscl;
    y *= yscl;

    hsize = 0.025 * (yymax - yymin);
    wsize = 0.025 * (xxmax - xxmin);

    symbl2( x, y, lab, strlen(lab), hsize, wsize, 0.0, 0.0, 0.5 );

    return;
}


#ifdef __cplusplus
extern "C"
#endif
void xgSetScale(
double x, double y)
{
    xscl = x;
    yscl = y;
    return;
}

#ifdef __cplusplus
extern "C"
#endif
void xgClear()
{
    gpend();
    gclear();
    axis_done = 0;
    cur_line = 1;
    line_type = 1;
    return;
}


#ifdef __cplusplus
extern "C"
#endif
void xgUpdate(
int reset_axis)
{
    ggtoa();
    if (reset_axis) gpend();
    return;
}

/************************************************************************
 *									*
 *	axis_2d(xxmin, xmax, yymin, ymax, xscale, yscale, pxmax, pymax,   *
 *              no_fill, flip, xlab, ylab) - 				*
 *  This routine draws a two dimensional axis for the two d plots.  It 	*
 *  labels and adds tic marks.						*
 *									*
 *	Original:	MEL	Nov, 1984				*
 *									*
 ************************************************************************/
void axis_2d(
float xxmin, float xxmax,	/*limits for the x axis*/
float yymin, float yymax,	/*limits for the y axis*/
float xscale, float yscale,   /*scaling to be applied to the labels*/
float pxmax, float pymax,	/*physical screen size*/
int flip,		/*flip the y axis upside down*/
char *xlab, char *ylab,	/*axis labels*/
int ylog)		/*draw y axis with a log scale*/
{
    int minor_tic;		/*number of tic marks*/
    float dist;		/*distance between tic marks*/
    float sxmin, sxmax;	/*screen location of start and stop of axis*/
    float symin, symax; /*screen location of start and stop of axis*/
    float lx, ly;	/*length of x and y axis*/
    float hgt;
    float xst, yst;	/*start of axis*/
    float len;		/*length of axis*/
    float axang;	/*axis angle*/
    float Fval, Eval;	/*axis value at start, end*/
    float Flab, Llab;	/*first, last labeled value*/
    float ltic; 	/*value of labeled ticks*/
    int utic;		/*number of tics between*/
    float labht, tlang;	/*tic label height, angle*/
    int tloc; 		/*tic label location flag*/
    float tang;		/*tic labels angle to draw at*/
    float tht, tdp;	/*height and depth of tics*/
    float axtht;	/*axis title height*/
    int lform;		/*location of title*/
    char *vform;	/*the tic label format string*/

    sxmin = 0.20 * pxmax;
    symin = 0.08 * pymax;
    sxmax = 0.97 * pxmax;
    symax = 0.95 * pymax;
    lx = sxmax - sxmin;
    ly = symax - symin;

    hgt = 0.02 * ly;

    Fval = xxmin;
    Eval = xxmax;
    axtcs2(Fval, Eval, 0, 0, &Flab, &Llab, &dist, &minor_tic);

    /* Let's start with some identification */
    /* Yaxis logic: hgt=0.02 ly, chars=0.025 + offset 0.5*char, margin=0.05 */
    if ( tlab != NULL )
    symb2(sxmin + lx/2,	/* Half way along the x axis */
	   symax,		/* Top of the y axis */
	   tlab,
	   strlen(tlab),	/* Is symb2 stupid or what? */
	   1.25*hgt,		/* Char size */
	   0.0,			/* No rotation */
	   -0.5,		/* hornt: (x,y) is bottom of string */
	   0.5);		/* wornt: (x,y) is center of string */

    /*do it*/
    gnline(1);
    axplt2(sxmin,	/*start of axis in physical coordinates, x*/
	   symin,	/*start of axis in physical coordinates, y*/
	   lx,		/*length of the axis in plot units*/
	   0.0,		/*angle that the axis is drawn at*/
	   Fval,	/*axis value at start location*/
	   Eval,	/*axis value at end location*/
	   Flab,	/*first labeled value on the axis*/
	   Llab,	/*last labeled value on the axis*/
	   dist,	/*distance between major tic marks*/
	   minor_tic,	/*number of minor tics between labels*/
	   hgt,		/*height of the labels on the axis*/
	   0.0,		/*angle for the labels*/
	   0,		/*flag - labels on bottom of axis*/
	   90.0,	/*angle at which tics are to be drawn*/
	   hgt,		/*height of major tic marks*/
	   hgt*0.5,	/*depth of major tic marks*/
	   xlab,	/*title of the axis*/
	   hgt,		/*title height*/
	   -1,		/*location of the title*/
	   "4.2f");	/*format for the labels*/
    

    /*do the y axis*/
    if ( ! flip ) {
	xst = sxmin;
	yst = symin;
	axang = 90.0;
	tlang = 270.0;
	tloc = 1;
	tang = 270.0;
	lform = 1;
    }
    else {
	xst = sxmin;
	yst = symax;
	axang = 270.0;
	tlang = 90.0;
	tloc = 0;
	tang = 270.0;
	lform = -2;
    }
    hgt = 0.02 * lx;

    if ( !ylog ) {
	Fval = yymin;
	Eval = yymax;
    } else {
	Fval = pow( 10.0, yymin * yscale);
	Eval = pow( 10.0, yymax * yscale);
    }
    axtcs2(Fval, Eval, 0, ylog, &Flab, &Llab, &ltic, &utic);


    /*do it*/
    axplt2(xst,			/*start of axis in physical coordinates*/
	   yst,			/*end of axis in physical coordinates*/
	   ly,	 		/*length of the axis in plot units*/
	   axang,		/*angle that the axis is drawn at*/
	   Fval,		/*axis value at start location*/
	   Eval,		/*axis value at end location*/
	   Flab,		/*first labeled value on the axis*/
	   Llab,		/*last labeled value on the axis*/
	   ltic,		/*distance between major tic marks*/
	   utic,		/*number of minor tics between labels*/
	   hgt,			/*height of the labels on the axis*/
	   tlang,		/*angle for the labels*/
	   tloc,		/*flag - labels on bottom of axis*/
	   tang,		/*angle at which tics are to be drawn*/
	   hgt,			/*height of major tic marks*/
	   hgt*0.5,		/*depth of major tic marks*/
	   ylab,		/*title of the axis*/
	   hgt,			/*title height*/
	   lform,		/*location of the title*/
	   "5.2g");		/*format for the labels*/

    gtrans(sxmin, symin);
    if ( ! flip) {
	gscale( lx / (xxmax - xxmin), ly / (yymax - yymin));
	gtrans(-xxmin, -yymin);
    }
    else {
	gscale( lx / (xxmax - xxmin), ly / (yymax - yymin));
	gtrans(-xxmin, yymax);
    }

    return;
}


#undef gmove
#undef gdraw

void gmove( 
float x, float y) 
{
    gplot2(G_MOVE,0,x,y);
    return;
}
void gdraw( 
float x, float y) 
{
    gplot2(G_DRAW,0,x,y);
    return;
}
