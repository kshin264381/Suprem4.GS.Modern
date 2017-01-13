/*
 * Copyright 1989 O'Reilly and Associates, Inc.
 * See ../Copyright for complete rights and liability information.
 */

/*
 * xbitmap1.c
 */

#include <math.h>

#include "GraphP.h"
#include "interface.h"
#include "plot.h"

extern XtAppContext help_context;
extern XtAppContext app_context;
extern GraphWin* newwin;
extern int toolkitinit;
extern DataSet* curspot;
extern PointList* curpt;
extern int newGroup;
extern GraphWidget graph;
extern double xg_xscl;
extern double xg_yscl;
extern Widget global;

extern void make_graph( GraphWin* win );

int xgNewSetLT(int line_spec)
{
    //newwin-> ... errrrr nothing to be done here.
    return 0;
}

GraphWin *make_datawin()
{
    GraphWin *tmp;

    tmp = (GraphWin *)malloc(sizeof(GraphWin));
    tmp->logx = tmp->logy = 0;
    tmp->Data = NULL;
    tmp->Label = NULL;
    tmp->Xsegs = NULL;
    tmp->max_segs = 0;
    tmp->Title = tmp->XUnitText = tmp->YUnitText = NULL;

    return(tmp);
}

void xgPlotInit()
{
    help_context = app_context = NULL;
    newwin = NULL;
    toolkitinit = FALSE;
}

void check_x( )
{
    XEvent event_return;

    while( app_context && XtAppPending(app_context)) {
        XtAppNextEvent(app_context, &event_return);
        XtDispatchEvent(&event_return);
    }
    while( help_context && XtAppPending(help_context)) {
        XtAppNextEvent(help_context, &event_return);
        XtDispatchEvent(&event_return);
    }
}

void xgSetBounds(double lowX, double upX, double lowY, double upY)
{
    double pad;

    if (!newwin) {
        newwin = make_datawin();
    }

    newwin->loX = lowX;
    newwin->hiX = upX;
    newwin->loY = lowY;
    newwin->hiY = upY;

    /* Increase the padding for aesthetics */
    if (newwin->hiX - newwin->loX == 0.0) {
        pad = MAX(0.5, fabs(newwin->hiX/2.0));
        newwin->hiX += pad;
        newwin->loX -= pad;
    }
    if (newwin->hiY - newwin->loY == 0) {
        pad = MAX(0.5, fabs(newwin->hiY/2.0));
        newwin->hiY += pad;
        newwin->loY -= pad;
    }

    /* Add 10% padding to bounding box (div by 20 yeilds 5%) */
    pad = (newwin->hiX - newwin->loX) / 20.0;
    newwin->loX -= pad;  newwin->hiX += pad;
    pad = (newwin->hiY - newwin->loY) / 20.0;
    newwin->loY -= pad;  newwin->hiY += pad;

    newwin->llx = newwin->loX;
    newwin->urx = newwin->hiX;
    newwin->lly = newwin->loY;
    newwin->ury = newwin->hiY;
}

void xgLogAxis(int xlog, int ylog)
{
    if (!newwin) {
        newwin = make_datawin();
    }
    newwin->logx = xlog;
    newwin->logy = ylog;
}

#define STRDUP(str)       (strcpy((char*)malloc((unsigned) (strlen(str)+1)), (str)))

void xgAxisLabels(char* xl, char* yl, char* ti)
{
    if (!newwin) {
        newwin = make_datawin();
    }
    if ( newwin->XUnitText ) {free(newwin->XUnitText); newwin->XUnitText=NULL;}
    if ( newwin->YUnitText ) {free(newwin->YUnitText); newwin->YUnitText=NULL;}
    if ( newwin->Title ) {free(newwin->Title); newwin->Title=NULL;}

    newwin->XUnitText = STRDUP(xl);
    newwin->YUnitText = STRDUP(yl);
    newwin->Title = STRDUP(ti);
}

int xgNewSet()
/*
 * Set up new dataset.  Will return zero if there are too many data sets.
 */
{
    DataSet *loc;

    if (!newwin) {
        newwin = make_datawin();
    }

    if (!newwin->Data) {
        newwin->Data = loc = (DataSet *)malloc(sizeof(DataSet));
    }
    else {
	/*walk to the end of the Data linked list*/
        for(loc = newwin->Data; loc->next != NULL; loc = loc->next);

        loc->next = (DataSet *)malloc(sizeof(DataSet));
        loc = loc->next;
    }

    curspot = loc;
    curspot->next = (DataSet *)NULL;
    curspot->list = (PointList *)NULL;
    curspot->marker  = 0;
    curspot->setName = (char *)NULL;
    curpt = NULL;
    newGroup = 1;

    return 0;
}

void xgSetName(char* name)
/*
 * Sets the name of a data set.  Automatically makes a copy.
 */
{
    if (!curspot) {
        curspot = (DataSet*)malloc(sizeof(DataSet));
    }
    curspot->setName = STRDUP(name);
}

void xgSetValue(double val)
{
    if (!curspot) {
        curspot = (DataSet*)malloc(sizeof(DataSet));
    }
    curspot->setValue = val;
}

void xgSetMark(int mark)
{
    if (!curspot) {
        curspot = (DataSet*)malloc(sizeof(DataSet));
    }
    curspot->marker = mark;
}

void xgNewGroup()
/*
 * Set up for reading new group of points within a dataset.
 */
{
    newGroup = 1;
}

#define INITSIZE 4
void xgPoint(double xval, double yval)
/*
 * Adds a new point to the current group of the current
 * data set.
 */
{
    if ( curpt == NULL ) {
        curspot->list = (PointList *)malloc(sizeof(PointList));
        curpt = curspot->list;
        curpt->numPoints = 0;
        curpt->allocSize = INITSIZE;
        curpt->xvec = (double *) malloc((unsigned)(INITSIZE * sizeof(double)));
        curpt->yvec = (double *) malloc((unsigned)(INITSIZE * sizeof(double)));
        curpt->next = (PointList *) 0;
        newGroup = 0;
    }
    else if (newGroup) {
        curpt->next = (PointList *) malloc(sizeof(PointList));
        curpt = curpt->next;
        curpt->numPoints = 0;
        curpt->allocSize = INITSIZE;
        curpt->xvec = (double *) malloc((unsigned)(INITSIZE * sizeof(double)));
        curpt->yvec = (double *) malloc((unsigned)(INITSIZE * sizeof(double)));
        curpt->next = (PointList *) 0;
        newGroup = 0;
    }

    if (curpt->numPoints >= curpt->allocSize) {
        curpt->allocSize *= 2;
        curpt->xvec = (double *) realloc((char *) curpt->xvec,
                       (unsigned) (curpt->allocSize *
                                   sizeof(double)));
        curpt->yvec = (double *) realloc((char *) curpt->yvec,
                       (unsigned) (curpt->allocSize *
                                   sizeof(double)));
    }

    curpt->xvec[curpt->numPoints] = xval * newwin->xg_xscl;
    curpt->yvec[curpt->numPoints] = yval * newwin->xg_yscl;

    (curpt->numPoints)++;
}

void xgLabel(char* lab, double x, double y)
{
    LabelList *spot;

    if (!newwin) {
        newwin = make_datawin();
    }

    if (!newwin->Label) {
        newwin->Label = (LabelList *)malloc(sizeof(LabelList));
        spot = newwin->Label;
    }
    else {
        for( spot = newwin->Label; spot->next; spot = spot->next );
        spot->next = (LabelList *)malloc(sizeof(LabelList));
        spot = spot->next;
    }

    spot->x = x * newwin->xg_xscl;
    spot->y = y * newwin->xg_yscl;
    spot->just = T_CENTER;
    spot->style = T_AXIS;
    spot->label = STRDUP(lab);
    spot->next = NULL;
}

void xgSetScale(double x, double y)
{
    if ( newwin == NULL) {
        newwin = make_datawin();
    }
    newwin->xg_xscl = x;
    newwin->xg_yscl = y;
}

void xgClear()
{
    int idx;
    DataSet *dloc, *dtmp;
    PointList *spot, *tmp;
    LabelList *spl, *tl;

    if (!newwin) return;

    if (newwin->Title)     {free(newwin->Title); newwin->Title=NULL;}
    if (newwin->XUnitText) {free(newwin->XUnitText); newwin->XUnitText=NULL;}
    if (newwin->YUnitText) {free(newwin->YUnitText); newwin->YUnitText=NULL;}

    /* Initialize the data sets */
    for (dloc = newwin->Data; dloc; ) {
        for(spot = dloc->list; spot; ) {
            tmp = spot;
            spot = spot->next;
            free(tmp->xvec); tmp->xvec=NULL;
            free(tmp->yvec); tmp->yvec=NULL;
            free(tmp);
        }
        if (dloc->setName) {free(dloc->setName); dloc->setName=NULL;}
        dtmp = dloc;
        dloc = dloc->next;
        free(dtmp);
    }
    newwin->Data = NULL;

    /*free the label space*/
    spl = newwin->Label;
    while(spl) {
        tl = spl;
        spl = spl->next;
        free(tl->label); tl->label=NULL;
        free(tl);
    }
    newwin->Label=NULL;

    /*clear the scaling factors*/
    newwin->xg_xscl = 1.0;
    newwin->xg_yscl = 1.0;

    XClearWindow(XtDisplay(graph), XtWindow(graph));
}

void xgUpdate( int reset_axis )
{
    int flags;
    void Traverse();

    if ( reset_axis && app_context ) {
        XClearWindow(XtDisplay(graph), XtWindow(graph));
    }

    if (!app_context) {
        make_graph(newwin);
    }

    if (newwin) {
        do_redraw(graph, newwin);
    }
}

void gdraw(double x, double y)
{
    xgPoint(x, y);
}

void gmove(double x, double y)
{
    xgNewGroup();
    xgPoint(x, y);
}

