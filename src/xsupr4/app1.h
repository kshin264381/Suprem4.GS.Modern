/*
 * app1.h
 *
 *  Created on: Mar 25, 2016
 *      Author: kshin
 */

#ifndef SRC_XSUPR4_APP1_H_
#define SRC_XSUPR4_APP1_H_


#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>

#include <X11/Shell.h>
#ifndef ardent
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>

#include <X11/Xaw/AsciiText.h>
#else
#include <X11/Dialog.h>
#include <X11/VPaned.h>
#include <X11/Box.h>
#include <X11/Command.h>
#include <X11/Toggle.h>
#include <X11/AsciiText.h>
#endif
#define check_width 16
#define check_height 16
static char check_bits[] = {
   0x00, 0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x20, 0x00, 0x30, 0x00, 0x18,
   0x00, 0x08, 0x00, 0x0c, 0x00, 0x04, 0x01, 0x06, 0x03, 0x03, 0x06, 0x01,
   0x8c, 0x01, 0xd8, 0x00, 0xf0, 0x00, 0x60, 0x00};

#include <stdlib.h>

//#include "GraphP.h"

//static void do_label(Widget widget, XtPointer client_data, XtPointer call_data);
//
//static void Printout(Widget widget, XtPointer client_data, XtPointer call_data);
//
//static void cleanup(Widget widget, XtPointer client_data, XtPointer call_data);
//
//void make_graph( GraphWin *win );
//
//void make_print_dialog(Widget print_shell, GraphWidget gw);
//
//static void cancelLabel(Widget widget, XtPointer client_data, XtPointer call_data);
//
//void make_lab_dialog(Widget lab_shell, GraphWin* win, GraphWidget graph);
//
//void wtogw(Widget w, GraphWidget gw);
//
//void do_error(char* str);

#endif /* SRC_XSUPR4_APP1_H_ */
