/*
 * xgX.h
 *
 *  Created on: Mar 25, 2016
 *      Author: kshin
 */

#ifndef SRC_XSUPR4_XGX_H_
#define SRC_XSUPR4_XGX_H_

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>

//#include "GraphP.h"

static unsigned char dashlist[MAXCOL][2] = {
	{1, 1},
	{2, 2},
	{3, 1},
	{7, 7},
	{4, 4},
	{4, 1},
	{3, 2},
	{3, 3}
};

#define PADDING 	2
#define SPACE 		10
#define TICKLENGTH	5
#define MAXSEGS		1000

//void segGC(Widget w, Pixel l_fg, int l_style, int l_width, unsigned char* l_chars, int l_len);
//
//void set_X(Widget new_stuff);
//
//void text_X(Widget w, int x, int y, char* text, int just, int style);
//
//void seg_X(Widget w, int ns, XSegment* segs, int width, int style, int lappr, int color);
//
//void make_markers(GraphWidget w);
//void dot_X(Widget w, int x, int y, int style, int type, int color);


#endif /* SRC_XSUPR4_XGX_H_ */
