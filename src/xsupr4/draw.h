/*
 * draw.h
 *
 *  Created on: Mar 25, 2016
 *      Author: kshin
 */

#ifndef SRC_XSUPR4_DRAW_H_
#define SRC_XSUPR4_DRAW_H_


#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "GraphP.h"

#define nlog10(x)      (x == 0.0 ? 0.0 : log10(x) + 1e-15)
#define ZERO_THRES 1.0e-5

/* Default line styles */
static char *defStyle[] = {
    "1", "10", "11110000", "010111", "1110",
    "1111111100000000", "11001111", "0011000111"
};

/* Default color names */
static char *defColors[] = {
    "red", "SpringGreen", "blue", "yellow",
    "cyan", "sienna", "orange", "coral"
};

#define LEFT_CODE   0x01
#define RIGHT_CODE  0x02
#define BOTTOM_CODE 0x04
#define TOP_CODE    0x08

/* Clipping algorithm from Neumann and Sproull by Cohen and Sutherland */
#define C_CODE(xval, yval, rtn) \
rtn = 0; \
if ((xval) < wi->UsrOrgX) rtn = LEFT_CODE; \
else if ((xval) > wi->UsrOppX) rtn = RIGHT_CODE; \
if ((yval) < wi->UsrOrgY) rtn |= BOTTOM_CODE; \
else if ((yval) > wi->UsrOppY) rtn |= TOP_CODE


//void do_redraw(Widget userdata, GraphWin* wi);
//
//void DrawTitle(Widget w, GraphWin* wi);
//
//void DrawGridAndAxis(Widget w, GraphWin* wi);
//
//double initGrid(double low, double step, int logFlag);
//
//double stepGrid();
//
//double RoundUp(double val);
//
//int DrawData(Widget w, GraphWin* wi);
//
//void DrawLabel(GraphWidget gw, GraphWin* wi);
//
//int DrawLegend(GraphWidget gw, GraphWin* wi);
//
//int TransformCompute(GraphWin* wi);
//
//void DrawNumber(Widget w, GraphWin* wi, double val, int x, int y, int just, int logflag);
//
//static int rdFindMax(GraphWin* wi);
//
//Widget gwtow(GraphWidget gw);
//
//GraphWidget writetogw(Widget w);

#endif /* SRC_XSUPR4_DRAW_H_ */
