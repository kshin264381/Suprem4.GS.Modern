#ifndef _HPGL_H_
#define _HPGL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <X11/Xlib.h>

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/CoreP.h>
#include <X11/Xregion.h>
#include <X11/Xaw/Toggle.h>

#include "DataRec.h"
#include "plotter.h"

//typedef struct {
//	double axis_w;
//	double axis_h;
//	double title_w;
//	double title_h;
//	FILE *plotterFile;
//	int clipminX;
//	int clipminY;
//	int clipmaxX;
//	int clipmaxY;
//} mydata;
//
//typedef mydata* MyData;
//
//typedef struct hpgl_out {
//    int dev_flags;		/* Device characteristic flags           */
//    int area_w;
//    int area_h;		    /* Width and height in pixels            */
//    int bdr_pad;		/* Padding from border                   */
//    int axis_pad;		/* Extra space around axis labels        */
//    int tick_len;		/* Length of tick mark on axis           */
//    int legend_pad;		/* Top of legend text to legend line     */
//    int axis_width;		/* Width of big character of axis font   */
//    int axis_height;		/* Height of big character of axis font  */
//    int title_width;		/* Width of big character of title font  */
//    int title_height;		/* Height of big character of title font */
//
//    void (*hpgl_init)();		/* Draws text at a location              */
//    void (*hpgl_text)(MyData, int, int, char*, int, int);		/* Draws text at a location              */
//    void (*hpgl_seg)(MyData, int, XSegment*, int, int, int, int);		/* Draws a series of segments            */
//    void (*hpgl_dot)(MyData, int, int, int, int, int);		/* Draws a dot or marker at a location   */
//    void (*hpgl_end)(MyData);		/* Stops the drawing sequence            */
//
//} hpglOut;
//
//void hpglInitDummy();
//
//int hpglInit(
//FILE* stream,
//int width,
//int height,
//char* title_family,
//double title_size,
//char* axis_family,
//double axis_size,
//int flags,
//hpglOut* outInfo,
//Widget* userinfo,
//char* errmsg);
//
//static void hpglText(MyData userState, int x, int y, char* text, int just, int style);
//
//static void hpglSeg(MyData userState, int ns, XSegment* segs, int width, int style, int lappr, int color);
//
//static void hpglDot(MyData userState, int x, int y, int style, int type, int color);
//
//static void hpglEnd(MyData userState);


#endif
