#include "app1.h"
#include "GraphP.h"
#include "plot.h"

// Replacing xgraph.h
XtAppContext app_context;
XtAppContext help_context;
GraphWidget graph;

GraphWin *newwin;

DataSet *curspot;
PointList *curpt;

double xg_xscl;
double xg_yscl;

int newGroup;

Widget global;

int toolkitinit;
// xgraph.h

extern void unzoom();



//void make_lab_dialog(Widget lab_shell, GraphWin win, GraphWidget graph);
//void make_print_dialog(Widget print_shell, GraphWidget gw);

static void do_label(Widget widget, XtPointer client_data, XtPointer call_data)
{
    Widget ps = (Widget)client_data;
    Position x, y;

    XtTranslateCoords(widget, 0, 0, &x, &y);
    XtVaSetValues(ps, XtNx, x+10, XtNy, y+10, NULL);
    XtPopup(ps, XtGrabNonexclusive);
}

static void Printout(Widget widget, XtPointer client_data, XtPointer call_data)
{
    Widget ps = (Widget)client_data;
    Position x, y;

    XtTranslateCoords(widget, 0, 0, &x, &y);
    XtVaSetValues(ps, XtNx, x+10, XtNy, y+10, NULL);
    XtPopup(ps, XtGrabNonexclusive);
}


static void cleanup(Widget widget, XtPointer client_data, XtPointer call_data)
{
    xgClear();
    XtDestroyApplicationContext(app_context);
    app_context = NULL;
    free(newwin);
    newwin = NULL;
}

extern void help_win();

void make_graph( GraphWin* win )
{
    Widget tL1, vpane, buttonbox;
    Widget unz, label, help, output;
    Widget device, print_shell, lab_shell, quit;
    int one = 1;
    char *argv[2];
    Display * dpy;

//    if (!graph) {
//        graph = (GraphWidget)malloc( sizeof(GraphRec) );
//    }
//    if (!win) {
//        win = init_GraphWin();
//    }
	
    argv[0] = "xsuprem";
    argv[1] = NULL;

    if ( !toolkitinit ) {
        XtToolkitInitialize();
        toolkitinit = TRUE;
    }
    app_context = XtCreateApplicationContext();

    dpy = XtOpenDisplay(app_context, (String) NULL, NULL,
            "XSuprem", NULL, 0, &one, argv);

    if (!dpy)
        XtErrorMsg("invalidDisplay","xtInitialize","XtToolkitError",
                   "Can't Open display", (String *) NULL, (Cardinal *)NULL);

    tL1 = XtVaAppCreateShell( NULL, "XSuprem", applicationShellWidgetClass, dpy,
            XtNscreen, (XtArgVal)DefaultScreenOfDisplay(dpy), NULL );
   
    vpane = XtVaCreateManagedWidget("vpane", panedWidgetClass, tL1, NULL);
    buttonbox = XtVaCreateManagedWidget("buttonbox", boxWidgetClass, vpane, NULL);
    graph = XtVaCreateManagedWidget("graph", graphWidgetClass, vpane, XtNGraphWin, win, NULL);

    /*Make the Help Button*/
    help = XtVaCreateManagedWidget("Help", commandWidgetClass, buttonbox, NULL);
    XtAddCallback(help, XtNcallback, help_win, NULL);

    /*Make the Label Button*/
    label = XtVaCreateManagedWidget("Axis", commandWidgetClass, buttonbox, NULL);
    lab_shell = XtVaCreatePopupShell("lshell", transientShellWidgetClass, tL1, NULL);
    make_lab_dialog(lab_shell, win, graph);
    XtAddCallback(label, XtNcallback, do_label, lab_shell);

    /*Make the unzoom button*/
    unz = XtVaCreateManagedWidget("Unzoom", commandWidgetClass, 
                buttonbox, NULL);
    XtAddCallback(unz, XtNcallback, unzoom, (XtPointer) graph);

    /*Make the HardCopy Button*/
    output = XtVaCreateManagedWidget("Print", commandWidgetClass, 
                buttonbox, NULL);

    print_shell = XtVaCreatePopupShell("pshell", transientShellWidgetClass,
                tL1, NULL);
    make_print_dialog(print_shell, graph);
    XtAddCallback(output, XtNcallback, Printout, print_shell);

    quit = XtVaCreateManagedWidget("Close", commandWidgetClass,
				    buttonbox, NULL);
    XtAddCallback(quit, XtNcallback, cleanup, NULL);

    XtRealizeWidget(tL1);
}


#include "HardInfo.h"
print_calldata foo, bar;

void make_print_dialog(Widget print_shell, GraphWidget gw)
{
    Widget vpane, bb, dialog;
    Widget temp, t1, t2;
    Widget name;

    foo.print_shell = print_shell;

    vpane = XtVaCreateManagedWidget("PrintDialog", panedWidgetClass, print_shell, NULL);

    temp = XtVaCreateManagedWidget("Device Dialog", labelWidgetClass, vpane, NULL);

    bb = XtVaCreateManagedWidget("buttonbox1", boxWidgetClass, vpane, NULL);


    foo.dev_file = XtVaCreateManagedWidget("Device", toggleWidgetClass, bb, 
					XtNradioData, DEVICE, 
					XtNstate, TRUE, NULL);

    t2 = XtVaCreateManagedWidget("Filename", toggleWidgetClass, bb, 
				  XtNradioGroup, foo.dev_file, 
				  XtNradioData, FILEN,
				  XtNstate, FALSE, NULL);

    bb = XtVaCreateManagedWidget("box1", boxWidgetClass, vpane, NULL);
    temp = XtVaCreateManagedWidget("Name: ", labelWidgetClass, bb, NULL);
    foo.dev_name = XtVaCreateManagedWidget("Name", asciiTextWidgetClass, bb, 
				    XtNeditType, XawtextEdit, 
				    NULL);

    bb = XtVaCreateManagedWidget("buttonbox2", boxWidgetClass, vpane, NULL);
    foo.dev_type = XtVaCreateManagedWidget("Postscript", toggleWidgetClass, bb, 
					XtNradioData, PS_,
					XtNstate, TRUE, NULL);
    temp = XtVaCreateManagedWidget("Idraw", toggleWidgetClass, bb, 
				        XtNradioGroup, foo.dev_type, 
					XtNradioData, IDRAW, 
					XtNstate, FALSE, NULL);
    temp = XtVaCreateManagedWidget("HPGL", toggleWidgetClass, bb, 
				        XtNradioGroup, foo.dev_type, 
					XtNradioData, HPGL, 
					XtNstate, FALSE, NULL);

    bb = XtVaCreateManagedWidget("buttonbox2", boxWidgetClass, vpane, NULL);
    foo.do_include = XtVaCreateManagedWidget("Include in Document", 
					toggleWidgetClass, bb,
					FALSE, NULL);

    bb = XtVaCreateManagedWidget("buttonbox2", boxWidgetClass, vpane, NULL);

    temp = XtVaCreateManagedWidget("Print", commandWidgetClass, bb, NULL);
    foo.graph = gw;
    XtAddCallback(temp, XtNcallback, do_pr, &foo);

    temp = XtVaCreateManagedWidget("Cancel", commandWidgetClass, bb, NULL);
    XtAddCallback(temp, XtNcallback, do_cn, &foo);
}

static void cancelLabel(Widget widget, XtPointer client_data, XtPointer call_data)
{
    Widget ps = (Widget)client_data;
    XtPopdown(ps);
}



extern void add_label();



void make_lab_dialog(Widget lab_shell, GraphWin* win, GraphWidget graph)
{
    Widget vpane, bb, temp;

    // If GraphWin actually exists
//    if (!win)
//        win = make_datawin();

    vpane = XtVaCreateManagedWidget(
            "AxisLabels", panedWidgetClass, lab_shell, NULL);

    temp = XtVaCreateManagedWidget(
            "Axis Labels", labelWidgetClass, vpane, NULL);

    bb = XtVaCreateManagedWidget("bb1", boxWidgetClass, vpane, NULL);
    temp = XtVaCreateManagedWidget("X Axis: ", labelWidgetClass, bb, NULL);
    bar.dev_file = XtVaCreateManagedWidget("Name", asciiTextWidgetClass, bb, 
				    XtNstring, win->XUnitText,
				    XtNeditType, XawtextEdit, 
				    NULL);

    bb = XtVaCreateManagedWidget("bb2", boxWidgetClass, vpane, NULL);
    temp = XtVaCreateManagedWidget("Y Axis: ", labelWidgetClass, bb, NULL);
    bar.dev_type = XtVaCreateManagedWidget("Name", asciiTextWidgetClass, bb, 
				    XtNstring, win->YUnitText,
				    XtNeditType, XawtextEdit, 
				    NULL);

    bb = XtVaCreateManagedWidget("bb3", boxWidgetClass, vpane, NULL);
    temp = XtVaCreateManagedWidget("Title: ", labelWidgetClass, bb, NULL);
    bar.dev_name = XtVaCreateManagedWidget("Name", asciiTextWidgetClass, bb, 
				    XtNstring, win->Title,
				    XtNeditType, XawtextEdit, 
				    NULL);

    bb = XtVaCreateManagedWidget("bb4", boxWidgetClass, vpane, NULL);

    bar.graph = graph;
    temp = XtVaCreateManagedWidget("Done", commandWidgetClass, bb, NULL);
    XtAddCallback(temp, XtNcallback, add_label, &bar);
    XtAddCallback(temp, XtNcallback, cancelLabel, lab_shell);

    temp = XtVaCreateManagedWidget("Cancel", commandWidgetClass, bb, NULL);
    XtAddCallback(temp, XtNcallback, cancelLabel, lab_shell);
}


void do_error(char* str)
{
    printf("%s\n", str);
}

//void wtogw(Widget w, GraphWidget gw)
//{
//    gw->core = w->core;
//}

//void gwtow(GraphWidget gw, Widget w)
//{
//    w->core = gw->core;
//}
