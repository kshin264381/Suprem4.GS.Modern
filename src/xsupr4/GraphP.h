/*
 * GraphP.h - Private definitions for Graph widget
 */

#ifndef _ORAGraphP_h
#define _ORAGraphP_h

/*
 * This include not needed unless the .c file includes IntrinsicP.h
 * after this file.   Anyway, it doesn't hurt.
 */
#include <X11/Xlib.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/CoreP.h>
#include <X11/Xregion.h>
#include <X11/Xaw/Toggle.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

/*
 * This one is always needed!
 */
#include "Graph.h"
#include "DataRec.h"

/* New fields for the Graph widget class record */

typedef struct {
    int make_compiler_happy;	/* keep compiler happy */
} GraphClassPart;

/* Full class record declaration */
typedef struct _GraphClassRec {
    CoreClassPart	core_class;
    GraphClassPart	graph_class;
} GraphClassRec;

extern GraphClassRec graphClassRec;
#define MAXCOL 8

/* New fields for the Graph widget record */
typedef struct {
    /* resources */
    XtCallbackList callback;	/* application installed callback function(s) */
    XFontStruct *titleFont;
    XFontStruct *labelFont;
    Pixel gridColor;
    Pixel zeroColor;
    Pixel pix[MAXCOL];		/*line colors*/
    GraphWin *wi;

    /* private state */
    int stx, sty;		/* start of zoom box */
    int crx, cry;		/* end of zoom box */
    Pixmap marker[MAXCOL];	/* line markers */
    GC		title_gc;	/* text label graphic context*/
    GC		label_gc;	/* text label graphic context*/
    GC		seg_gc;		/* segment drawing context*/
    GC		dot_gc;		/* dots */
    GC		echo_gc;	/* for the outlines */
} GraphPart;

/*
 * Full instance record declaration
 */
typedef struct _GraphRec {
    CorePart		core;
    GraphPart		graph;
} GraphRec;

/* Declaration of methods */

/* Graph.c */
void associate(
    GraphWidget gw,
    GraphWin *wi);

static void Initialize(
    Widget treq,
    Widget tnew,
    ArgList args,
    Cardinal *num_args);

static void GraphRedisplay(
    Widget w,
    XEvent *event);

static void Destroy(
    Widget w);

static void Resize(
    Widget w);

static void DrawCell(
    Widget w,
    XEvent *event);

static Boolean SetValues(
    Widget current,
    Widget request,
    Widget new_stuff,
    ArgList args,
    Cardinal *num_args);

static XtGeometryResult QueryGeometry(
    Widget w,
    XtWidgetGeometry *proposed,
    XtWidgetGeometry *answer);

void unzoom(
    Widget widget,
    XtPointer client_data,
    XtPointer call_data);

void do_cn(
    Widget widget,
    XtPointer client_data,
    XtPointer call_data);

void do_pr(
    Widget widget,
    XtPointer client_data,
    XtPointer call_data);

static void tildeExpand(
    char *out,
    char *in);

void add_label(
    Widget widget,
    XtPointer client_data,
    XtPointer call_data);

// Some makeshift initializers
xgOut init_xgOut();
void graphInit(GraphWidget gw);
GraphWin* init_GraphWin();

/* draw.h */
void DrawTitle(Widget w, GraphWin* wi);

void DrawGridAndAxis(Widget w, GraphWin* wi);

double initGrid(double low, double step, int logFlag);

double stepGrid();
double RoundUp(double val);
int DrawData(Widget w, GraphWin* wi);
void DrawLabel(GraphWidget gw, GraphWin* wi);
int DrawLegend(GraphWidget gw, GraphWin* wi);
int TransformCompute(GraphWin* wi);
void DrawNumber(Widget w, GraphWin* wi, double val, int x, int y, int just, int logflag);
int rdFindMax(GraphWin* wi);

//Widget gwtow(GraphWidget gw);
//
//GraphWidget writetogw(Widget w);

void do_redraw(Widget userdata, GraphWin* wi);

/* xgX.h */
void segGC(Widget w, Pixel l_fg, int l_style, int l_width, char* l_chars, int l_len);

void set_X(Widget new_stuff);

void text_X(Widget w, int x, int y, char* text, int just, int style);

void seg_X(Widget w, int ns, XSegment* segs, int width, int style, int lappr, int color);

void make_markers(GraphWidget w);
void dot_X(Widget w, int x, int y, int style, int type, int color);

void dummy_end_X(Widget w);

/* ps.h */
int rd(double dbl);

int psInit(
    FILE* psFile,
    int width,
    int height,
    char* tf,
    double ts,
    char* af,
    double as,
    int flags,
    xgOut* outInfo,
    Widget* userdata,
    char* errmsg);

static void psHeader(FILE* psFile, int docu_flag);

static void psScale(FILE* psFile, int width, int height, int flags);

static void psFonts(FILE* psFile);

static void psMarks(FILE* psFile);

static void psText(Widget state, int x, int y, char* text, int just, int style);

static void psSeg(Widget state, int ns, XSegment* seglist, int width, int style, int lappr, int color);

static void psDot(Widget state, int x, int y, int style, int type, int color);

static void psEnd(Widget userState);


/* interface.h */
GraphWin* make_datawin();

/* app1.h */
static void do_label(Widget widget, XtPointer client_data, XtPointer call_data);

static void Printout(Widget widget, XtPointer client_data, XtPointer call_data);

static void cleanup(Widget widget, XtPointer client_data, XtPointer call_data);

void make_graph( GraphWin *win );

void make_print_dialog(Widget print_shell, GraphWidget gw);

static void cancelLabel(Widget widget, XtPointer client_data, XtPointer call_data);

void make_lab_dialog(Widget lab_shell, GraphWin* win, GraphWidget graph);

//void wtogw(Widget w, GraphWidget gw);

void do_error(char* str);


/* hpgl.h */
typedef struct {
    double axis_w;
    double axis_h;
    double title_w;
    double title_h;
    FILE *plotterFile;
    int clipminX;
    int clipminY;
    int clipmaxX;
    int clipmaxY;
} mydata;

typedef mydata* MyData;

typedef struct hpgl_out {
    int dev_flags;		/* Device characteristic flags           */
    int area_w;
    int area_h;		    /* Width and height in pixels            */
    int bdr_pad;		/* Padding from border                   */
    int axis_pad;		/* Extra space around axis labels        */
    int tick_len;		/* Length of tick mark on axis           */
    int legend_pad;		/* Top of legend text to legend line     */
    int axis_width;		/* Width of big character of axis font   */
    int axis_height;		/* Height of big character of axis font  */
    int title_width;		/* Width of big character of title font  */
    int title_height;		/* Height of big character of title font */

    void (*hpgl_init)();		/* Draws text at a location              */
    void (*hpgl_text)(MyData, int, int, char*, int, int);		/* Draws text at a location              */
    void (*hpgl_seg)(MyData, int, XSegment*, int, int, int, int);		/* Draws a series of segments            */
    void (*hpgl_dot)(MyData, int, int, int, int, int);		/* Draws a dot or marker at a location   */
    void (*hpgl_end)(MyData);		/* Stops the drawing sequence            */

} hpglOut;

void hpglInitDummy();

int hpglInit(
    FILE* stream,
    int width,
    int height,
    char* title_family,
    double title_size,
    char* axis_family,
    double axis_size,
    int flags,
    hpglOut* outInfo,
    Widget* userinfo,
    char* errmsg);

static void hpglText(MyData userState, int x, int y, char* text, int just, int style);

static void hpglSeg(MyData userState, int ns, XSegment* segs, int width, int style, int lappr, int color);

static void hpglDot(MyData userState, int x, int y, int style, int type, int color);

static void hpglEnd(MyData userState);


/* idraw.h */
int idrawInit(
    FILE* strm,
    int width,
    int height,
    char* title_family,
    double title_size,
    char* axis_family,
    double axis_size,
    int flags,
    xgOut* out_info,
    Widget* userinfo,
    char* errmsg);

void idrawText(Widget user_state, int x, int y, char* text, int just, int style);

void idrawSeg(Widget user_state, int ns, XSegment* seglist, int width, int style, int lappr, int color);

void idrawDot(Widget user_state, int x, int y, int style, int type, int color);

void idrawEnd(Widget user_state);

/* help.h */
static void destroy_help(Widget widget, XtPointer client_data, XtPointer call_data);

void help_win(Widget widget, XtPointer client_data, XtPointer call_data);

#endif /* _ORAGraphP_h */
