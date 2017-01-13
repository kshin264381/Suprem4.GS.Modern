/*
 * Postscript output for xgraph
 *
 * Rick Spickelmier
 * David Harrison
 * ps.c 1.1
 */

#include "GraphP.h"
#include "ps.h"


int rd(double dbl)
//double dbl;
/* Short and sweet rounding function */
{
    if (dbl < 0.0) {
	return ((int) (dbl - 0.5));
    } else {
	return ((int) (dbl + 0.5));
    }
}

/*ARGSUSED*/
int psInit(FILE* psFile, int width, int height, char* tf, double ts, char* af, double as, int flags, xgOut* outInfo, Widget* userdata, char* errmsg)
//FILE *psFile;			/* Output file            */
//int width, height;		/* In microns             */
//char *tf, *af;			/* Title and axis font    */
//double ts, as;			/* Title and axis size    */
//int flags;			/* Predicate flags        */
//xgOut *outInfo;			/* Returned device info   */
//char **userdata;			/* User Data Information	*/
//char *errmsg;			/* Returned error message */
/*
 * The basic coordinate system is points (roughly 1/72 inch).
 * However,  most laser printers can do much better than that.
 * We invent a coordinate system based on VDPI dots per inch.
 * This goes along the long side of the page.  The long side
 * of the page is LDIM inches in length,  the short side
 * SDIM inches in length.  We we call this unit a `dev'.
 * We map `width' and `height' into devs.
 */
{
    struct userInfo *ui;
    double font_size;

    ui = (struct userInfo *) malloc(sizeof(struct userInfo));
    ui->psFile = psFile;
    ui->currentTextStyle = PS_NO_TSTYLE;
    ui->currentDashStyle = PS_NO_DSTYLE;
    ui->currentWidth = PS_NO_WIDTH;
    ui->currentLStyle = PS_NO_LSTYLE;
    ui->title_family = tf;
    ui->title_size = ts;
    ui->axis_family = af;
    ui->axis_size = as;
    /* Roughly,  one-eighth a point in devs */
    ui->baseWidth = rd( VDPI / POINTS_PER_INCH * BASE_WIDTH );
    ui->flags = flags;

    PS_scale = VDPI / MICRONS_PER_INCH;

    outInfo->dev_flags = 0;
    outInfo->area_w = rd( ((double) width) * PS_scale );
    outInfo->area_h = rd( ((double) height) * PS_scale );
    ui->height_devs = outInfo->area_h;
    outInfo->bdr_pad = rd( PS_BDR_PAD * VDPI );
    outInfo->axis_pad = rd( PS_AXIS_PAD * VDPI );
    outInfo->legend_pad = rd( PS_LEG_PAD * VDPI );
    outInfo->tick_len = rd( PS_TICK_LEN * VDPI );

    /* Font estimates */
    font_size = as * INCHES_PER_POINT * VDPI;
    outInfo->axis_height = rd( font_size );
    outInfo->axis_width = rd( font_size * FONT_WIDTH_EST );
    font_size = ts * INCHES_PER_POINT * VDPI;
    outInfo->title_height = rd( font_size );
    outInfo->title_width = rd( font_size * FONT_WIDTH_EST );

    outInfo->xg_text = psText;
    outInfo->xg_seg = psSeg;
    outInfo->xg_dot = psDot;
    outInfo->xg_end = psEnd;
    *userdata = (Widget)ui;

    /* Postscript file identification */
    PS("%%!\n");

    /* Definitions */
    psScale(psFile, width, height, flags);
    psFonts(psFile);
    psMarks(psFile);

    PS("%%\n%% Main body begins here\n%%\n");
    return 1;
}


static void psHeader(FILE* psFile, int docu_flag)
//FILE *psFile;
//int docu_flag;
/*
 * Prints out a standard greeting to the Postscript file.
 */
{
    PS("%%%%EndComments\n");
    PS("%%\n");
    PS("%% Xgraph postscript output\n");
    PS("%% Rick Spickelmier and David Harrison\n");
    PS("%% University of California, Berkeley\n");
    if (docu_flag) {
	PS("%%\n");
	PS("%% Output produced for inclusion in another document.\n");
	PS("%% This file will not work properly if sent directly to a printer.\n");
    }
    PS("%%\n");
}


static void psScale(FILE* psFile, int width, int height, int flags)
//FILE *psFile;			/* Output stream */
//int width;			/* Output width  */
//int height;			/* Output height */
//int flags;			/* Output options */
/*
 * This routine figures out how transform the basic postscript
 * transformation into one suitable for direct use by
 * the drawing primitives.  Two variables X-CENTER-PLOT
 * and Y-CENTER-PLOT determine whether the plot is centered
 * on the page.  If `flags' has D_DOCU set,  then the plot
 * will not be rotated or centered and a bounding box will
 * be displayed.
 */
{
    double factor;
    double pnt_width, pnt_height;

    if (flags & D_DOCU) {
	OUT(psFile, "%%%%BoundingBox: %ld %ld %ld %ld\n",
	    0, 0,
	    (int) (((double) width) /
		   (MICRONS_PER_INCH * INCHES_PER_POINT)+0.5),
	    (int) (((double) height) /
		   (MICRONS_PER_INCH * INCHES_PER_POINT)+0.5)
	    );
	psHeader(psFile, 1);
	PS("%% Rotation and centering are turned off for inclusion in a document\n");
    } else {
	psHeader(psFile, 0);
	PS("%% Scaling information\n");
	PS("%%\n");
	PS("%% Change these if you would like to change the centering\n");
	PS("%% of the plot in either dimension\n");
	PS("/X-CENTER-PLOT 1 def\n");
	PS("/Y-CENTER-PLOT 1 def\n");
	PS("%%\n");
	/*
	 * Determine page size
	 */
	PS("%% Page size computation\n");
	PS("clippath pathbbox\n");
	PS("/page-height exch def\n");
	PS("/page-width exch def\n");
	PS("pop pop\n");

	/*
	 * First: rotation.  If the width is greater than the short
	 * dimension,  do the rotation.
	 */
	pnt_width = ((double) width) / MICRONS_PER_INCH * POINTS_PER_INCH;
	pnt_height = ((double) height) / MICRONS_PER_INCH * POINTS_PER_INCH;
	PS("%% Determine whether rotation is required\n");
	OUT(psFile, "%lg page-width gt\n", pnt_width);
	PS("{ %% Rotation required\n");
	PS("   90 rotate\n");
	PS("   0 page-width neg translate\n");
	PS("   %% Handle centering\n");
	PS("   Y-CENTER-PLOT 1 eq { %% Center in y\n");
	OUT(psFile, "      page-height %lg sub 2 div\n", pnt_width);
	PS("   } { %% Don't center in y\n");
	PS("      0\n");
	PS("   } ifelse\n");
	PS("   X-CENTER-PLOT 1 eq { %% Center in x\n");
	OUT(psFile, "      page-width %lg sub 2 div\n", pnt_height);
	PS("   } { %% Don't center in x\n");
	PS("      0\n");
	PS("   } ifelse\n");
	PS("   translate\n");
	PS("} { %% No rotation - just handle centering\n");
	PS("   X-CENTER-PLOT 1 eq { %% Center in x\n");
	OUT(psFile, "      page-width %lg sub 2 div\n", pnt_width);
	PS("   } { %% Don't center in x\n");
	PS("      0\n");
	PS("   } ifelse\n");
	PS("   Y-CENTER-PLOT 1 eq { %% Center in y\n");
	OUT(psFile, "      page-height %lg sub 2 div\n", pnt_height);
	PS("   } { %% Don't center in y\n");
	PS("      0\n");
	PS("   } ifelse\n");
	PS("   translate\n");
	PS("} ifelse\n");
    }

    /*
     * Now: scaling.  We have points.  We want devs.
     */
    factor = POINTS_PER_INCH / VDPI;
    PS("%% Set the scale\n");
    OUT(psFile, "%lg %lg scale\n", factor, factor);
}


static void psFonts(FILE* psFile)
//FILE *psFile;			/* Output stream                */
/*
 * Downloads code for drawing title and axis labels
 */
{
    PS("%% Font Handling Functions\n");
    PS("%%\n");
    PS("%% Function giving y-offset to center of font\n");
    PS("%% Assumes font is set and uses numbers to gauge center\n");
    PS("%%\n");
    PS("/choose-font	%% stack: fontsize fontname => ---\n");
    PS("{\n");
    PS("   findfont \n");
    PS("   exch scalefont \n");
    PS("   setfont\n");
    PS("   newpath\n");
    PS("   0 0 moveto (0) true charpath flattenpath pathbbox\n");
    PS("   /top exch def pop\n");
    PS("   /bottom exch def pop\n");
    PS("   bottom top bottom top add 2 div\n");
    PS("   /center-font-val exch def \n");
    PS("   /upper-font-val exch def \n");
    PS("   /lower-font-val exch def\n");
    PS("} def\n");
    PS("%%\n");
    PS("%% Justfication offset routines\n");
    PS("%%\n");
    PS("/center-x-just	%% stack: (string) x y => (string) newx y\n");
    PS("{\n");
    PS("   exch 2 index stringwidth pop 2 div sub exch\n");
    PS("} def\n");
    PS("%%\n");
    PS("/left-x-just	%% stack: (string) x y => (string) newx y\n");
    PS("{ \n");
    PS("} def\n");
    PS("%%\n");
    PS("/right-x-just	%% stack: (string) x y => (string) newx y\n");
    PS("{\n");
    PS("   exch 2 index stringwidth pop sub exch\n");
    PS("} def\n");
    PS("%%\n");
    PS("/center-y-just	%% stack: (string) x y => (string) x newy\n");
    PS("{\n");
    PS("   center-font-val sub\n");
    PS("} def\n");
    PS("%%\n");
    PS("/lower-y-just	%% stack: (string) x y => (string) x newy\n");
    PS("{\n");
    PS("   lower-font-val sub\n");
    PS("} def\n");
    PS("%%\n");
    PS("/upper-y-just	%% stack: (string) x y => (string) x newy\n");
    PS("{\n");
    PS("   upper-font-val sub\n");
    PS("} def\n");
    PS("%%\n");
    PS("%% Shows a string on the page subject to justification\n");
    PS("%%   \n");
    PS("/just-string	%% stack: (string) x y just => ---\n");
    PS("{\n");
    PS("   dup 0 eq { pop center-x-just center-y-just 		} if\n");
    PS("   dup 1 eq { pop left-x-just center-y-just		} if\n");
    PS("   dup 2 eq { pop left-x-just upper-y-just	 	} if\n");
    PS("   dup 3 eq { pop center-x-just upper-y-just 		} if\n");
    PS("   dup 4 eq { pop right-x-just upper-y-just	 	} if\n");
    PS("   dup 5 eq { pop right-x-just center-y-just 		} if\n");
    PS("   dup 6 eq { pop right-x-just lower-y-just	 	} if\n");
    PS("   dup 7 eq { pop center-x-just lower-y-just  		} if\n");
    PS("   dup 8 eq { pop left-x-just lower-y-just	 	} if\n");
    PS("   moveto show\n");
    PS("} def\n");
    PS("%%\n");
}

static void psMarks(FILE* psFile)
/*
 * Writes out marker definitions
 */
{
    PS("%% Marker definitions\n");
    PS("/mark0 {/size exch def /y exch def /x exch def\n");
    PS("newpath x size sub y size sub moveto\n");
    PS("size size add 0 rlineto 0 size size add rlineto\n");
    PS("0 size size add sub 0 rlineto closepath fill} def\n");
    
    PS("/mark1 {/size exch def /y exch def /x exch def\n");
    PS("newpath x size sub y size sub moveto\n");
    PS("size size add 0 rlineto 0 size size add rlineto\n");
    PS("0 size size add sub 0 rlineto closepath stroke} def\n");
    
    PS("/mark2 {/size exch def /y exch def /x exch def\n");
    PS("newpath x y moveto x y size 0 360 arc stroke} def\n");

    PS("/mark3 {/size exch def /y exch def /x exch def\n");
    PS("newpath x size sub y size sub moveto x size add y size add lineto\n");
    PS("x size sub y size add moveto x size add y size sub lineto stroke} def\n");

    PS("/mark4 {/size exch def /y exch def /x exch def\n");
    PS("newpath x size sub y moveto x y size add lineto\n");
    PS("x size add y lineto x y size sub lineto\n");
    PS("closepath stroke} def\n");
    
    PS("/mark5 {/size exch def /y exch def /x exch def\n");
    PS("x y size mark1\n");
    PS("newpath x size sub y moveto size size add 0 rlineto stroke} def\n");
    
    PS("/mark6 {/size exch def /y exch def /x exch def\n");
    PS("newpath x y moveto x y size 0 360 arc fill} def\n");

    PS("/mark7 {/size exch def /y exch def /x exch def\n");
    PS("newpath x y moveto x size sub y size sub lineto\n");
    PS("x size add y size sub lineto closepath fill\n");
    PS("newpath x y moveto x size add y size add lineto\n");
    PS("x size sub y size add lineto closepath fill} def\n");
}



static void psText(Widget state, int x, int y, char* text, int just, int style)
//char *state;			/* Really (struct userInfo *) */
//int x, y;			/* Text position (devs)       */
//char *text;			/* Text itself                */
//int just;			/* Justification              */
//int style;			/* Style                      */
/*
 * Draws text at the given location with the given justification
 * and style.
 */
{
    struct userInfo *ui = (struct userInfo *) state;

    if (style != ui->currentTextStyle) {
	switch (style) {
	case T_AXIS:
	    OUT(ui->psFile, "%lg /%s choose-font\n",
		ui->axis_size * INCHES_PER_POINT * VDPI, ui->axis_family);
	    break;
	case T_TITLE:
	    OUT(ui->psFile, "%lg /%s choose-font\n",
		ui->title_size * INCHES_PER_POINT * VDPI, ui->title_family);
	    break;
	}
	ui->currentTextStyle = style;
    }
    OUT(ui->psFile, "(%s) %d %d %d just-string\n", text, x, IY(y), just);
}



/*ARGSUSED*/
static void psSeg(Widget state, int ns, XSegment* seglist, int width, int style, int lappr, int color)
//char *state;			/* Really (struct userInfo *) */
//int ns;				/* Number of segments         */
//XSegment *seglist;		/* X array of segments        */
//int width;			/* Width of lines (devcoords) */
//int style;			/* L_AXIS, L_ZERO, L_VAR      */
//int lappr;			/* Zero to seven              */
//int color;			/* Zero to seven              */
/*
 * Draws a number of line segments.  Grid lines are drawn using
 * light lines.  Variable lines (L_VAR) are drawn wider.  This
 * version ignores the color argument.
 */
{
    struct userInfo *ui = (struct userInfo *) state;
    int newwidth, i;

    if ((style != ui->currentLStyle) || (width != ui->currentWidth)) {
	switch (style) {
	case L_AXIS:
	    newwidth = PS_AXIS_WBASE * ui->baseWidth;
	    PSU("[] 0 setdash\n");
	    break;
	case L_ZERO:
	    newwidth = PS_ZERO_WBASE * ui->baseWidth;
	    PSU("[] 0 setdash\n");
	    break;
	case L_VAR:
	    newwidth = PS_DATA_WBASE * ui->baseWidth;
	    break;
	}
	ui->currentWidth = MAX(newwidth, width);
	ui->currentLStyle = style;
	OUT(ui->psFile, "%d setlinewidth\n", ui->currentWidth);
    }
    if ((lappr != ui->currentDashStyle) && (style == L_VAR)) {
	if (lappr == 0) {
	    PSU("[] 0 setdash\n");
	} else {
	    OUT(ui->psFile, "[%lg] 0 setdash\n",
		((double) lappr) * BASE_DASH * VDPI);
	}
	ui->currentDashStyle = lappr;
    }
    PSU("newpath\n");
    OUT(ui->psFile, "  %d %d moveto\n", seglist[0].x1, IY(seglist[0].y1));
    OUT(ui->psFile, "  %d %d lineto\n", seglist[0].x2, IY(seglist[0].y2));
    for (i = 1;  i < ns;  i++) {
	if ((seglist[i].x1 != seglist[i-1].x2) ||
	    (seglist[i].y1 != seglist[i-1].y2)) {
	    OUT(ui->psFile, "  %d %d moveto\n", seglist[i].x1, IY(seglist[i].y1));
	}
	OUT(ui->psFile, "  %d %d lineto\n", seglist[i].x2, IY(seglist[i].y2));
    }
    PSU("stroke\n");
}



/*ARGSUSED*/
static void psDot(Widget state, int x, int y, int style, int type, int color)
//char *state;    	/* state information */
//int x,y;    		/* coord of dot */
//int style;  		/* type of dot */
//int type;   		/* dot style variation */
//int color;  		/* color of dot */
/*
 * Prints out a dot at the given location
 */
{
    struct userInfo *ui = (struct userInfo *) state;

    if (ui->currentDashStyle != PS_NO_DSTYLE) {
	OUT(ui->psFile, "[] 0 setdash ");
	ui->currentDashStyle = PS_NO_DSTYLE;
    }
    if (ui->currentWidth != PS_ZERO_WBASE * ui->baseWidth) {
	ui->currentWidth = PS_ZERO_WBASE * ui->baseWidth;
	OUT(ui->psFile, "%d setlinewidth ", ui->currentWidth);
    }
    
    switch (style) {
    case P_PIXEL:
	OUT(ui->psFile, "newpath %d %d moveto %d %d %d 0 360 arc fill\n",
	    x, IY(y), x, IY(y), PS_PIXEL * ui->baseWidth);
	break;
    case P_DOT:
	OUT(ui->psFile, "newpath %d %d moveto %d %d %d 0 360 arc fill\n",
	    x, IY(y), x, IY(y), PS_DOT * ui->baseWidth);
	break;
    case P_MARK:
	OUT(ui->psFile, "%d %d %d mark%d\n",
	    x, IY(y), PS_MARK * ui->baseWidth, type);
	break;
    }
    return;
}


static void psEnd(Widget userState)
//char *userState;    /* state information */
{
    struct userInfo *ui = (struct userInfo *) userState;

    if (!(ui->flags & D_DOCU)) {
	PSU("showpage\n");
    }
    PSU("%% End of xgraph output\n");
    free(userState);
}
