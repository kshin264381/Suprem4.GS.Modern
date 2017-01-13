#ifndef _PS_H_
#define _PS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>

#include "DataRec.h"

/*
 * Basic scaling parameters
 */

#define VDPI			1200.0
#define LDIM			11.0
#define SDIM			8.5
#define MICRONS_PER_INCH	2.54E+04
#define POINTS_PER_INCH		72.0
#define INCHES_PER_POINT	1.0/72.0

/*
 * Aesthetic parameters (inches)
 */

#define PS_BDR_PAD		0.075
#define PS_AXIS_PAD		0.1
#define PS_LEG_PAD		0.025
#define PS_TICK_LEN		0.125
#define BASE_DASH		(1.0/48.0)

#define BASE_WIDTH		(1.0/8.0)
#define PS_AXIS_WBASE		1
#define PS_ZERO_WBASE		4
#define PS_DATA_WBASE		7
#define PS_PIXEL		4
#define PS_DOT			12
#define PS_MARK			12

/*
 * Other constants
 */

#define FONT_WIDTH_EST		0.55
#define PS_MAX_SEGS		1000
#define PS_NO_TSTYLE		-1
#define PS_NO_DSTYLE		-1
#define PS_NO_WIDTH		-1
#define PS_NO_LSTYLE		-1

/*
 * Working macros
 */

#define OUT		(void) fprintf
#define PS(str)		OUT(psFile, str)
#define PSU(str)	OUT(ui->psFile, str)
#define IY(val)		(ui->height_devs - val)

#ifndef MAX
#define MAX(a, b)	((a) > (b) ? (a) : (b))
#endif

/*
 * Globals
 */

static double PS_scale;		/* devs/micron */

/*
 * Externals and forwards
 */

//static void psScale(), psFonts(), psMarks(), psText(), psSeg(), psDot(), psEnd();

/*
 * Local structures
 */

struct userInfo {
    FILE *psFile;
    int currentTextStyle;
    int currentDashStyle;
    int currentWidth;
    int currentLStyle;
    int baseWidth;
    int height_devs;
    char *title_family;
    double title_size;
    char *axis_family;
    double axis_size;
    int flags;
};

//int rd(double dbl);
//
//int psInit(
//FILE* psFile,
//int width,
//int height,
//char* tf,
//double ts,
//char* af,
//double as,
//int flags,
//xgOut* outInfo,
//Widget* userdata,
//char* errmsg);
//
//static void psHeader(FILE* psFile, int docu_flag);
//
//static void psScale(FILE* psFile, int width, int height, int flags);
//
//static void psFonts(FILE* psFile);
//
//static void psMarks(FILE* psFile);
//
//static void psText(Widget state, int x, int y, char* text, int just, int style);
//
//static void psSeg(Widget state, int ns, XSegment* seglist, int width, int style, int lappr, int color);
//
//static void psDot(Widget state, int x, int y, int style, int type, int color);
//
//static void psEnd(Widget userState);



#endif
