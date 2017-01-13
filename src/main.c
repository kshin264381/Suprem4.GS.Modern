/*   %M%                Version %I%     */
/*   Last Modification : %G% %U%        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1994                                              */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/*-----------------------------------------------------------------*/

/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *     Copyright c 1991 The board of regents of the University of 	 *
 *                      Florida.  All rights reserved.			 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of the University of 	 *
 *     Florida.								 *
 *									 *
 *									 *
 *************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

#include <string.h>
#include <strings.h>
#include <malloc.h>
#include <stdlib.h>

#define STATIC_ALLOCATION_TIME
#include "data_types.h"
#include "global.h"
#include "check.h"
#include "constant.h"
#include "dbaccess.h"
#include "dbase.h"
#include "diffuse.h"
#include "impurity.h"
#include "defect.h"
#include "key.h"
#include "material.h"
#include "matrix.h"
#include "plot.h"
#include "regrid.h"
#include "shell.h"
#include "implant.h"
#include "newimplant.h"
#include "sysdep.h"
#include "expr.h"
#include "refine.h"
#include "poly.h"
#ifdef DEVICE
#include "device.h"
#endif
#include "misc.h"
#include "dislocation.h"

/*signal location of return*/
//extern int onintr();

/*every command subroutine has to be declared here*/
extern void echo(char *, par_str*);
extern void cpu(char *, par_str*);
extern void man(char *, par_str*);
extern void plot_2d(char *, par_str*);
extern void option(char *, par_str*);
extern void method(char *, par_str*);
extern void diffuse(char *,par_str*);
#ifdef NEWIMPLANT
extern void newimplant(char *, par_str*);
#else
extern void implant(char *, par_str*);
#endif
extern void sel_var(char *, par_str*);
extern void contour(char *, par_str*);
extern void plot_1d(char *, par_str*);
extern void print_1d(char *, par_str*);
extern void vacancy(char *, par_str*);
extern void interstitial(char *, par_str*);
extern void antimony(char *, par_str*);
extern void arsenic(char *, par_str*);
extern void boron(char *, par_str*);
//extern void regrid(char *, par_str*);
extern void coeffox(char *, par_str*);
extern void phosphorus(char *, par_str*);
extern void rregion(char *, par_str*);
extern void redge(char *, par_str*);
extern void rline(char *, par_str*);
extern void initialize(char *, par_str*);
extern void structure(char *, par_str*);
extern void user_deposit(char *, par_str*);
extern void user_etch(char *, par_str*);
extern void gold(char *, par_str*);
//extern void draw_lab(char *, par_str*);
extern void stress_analysis(char *, par_str*);
extern void c_mater(char *, par_str*);
extern void cesium(char *, par_str*);
extern void uPause(char *, par_str*);
extern void trap(char *, par_str*);
extern void user_print(char *, par_str*);
extern void profile(char *, par_str*);
extern void mode_cmd(char *, par_str*);
extern void device(char *, par_str*);
extern void beryllium(char *, par_str*);
extern void magnesium(char *, par_str*);
extern void selenium(char *, par_str*);
extern void silicon(char *, par_str*);
extern void tin(char *, par_str*);
extern void germanium(char *,par_str*);
extern void zinc(char *, par_str*);
extern void carbon(char *, par_str*);
extern void generic(char *, par_str*);
extern void dislocation(char *, par_str*);

struct command_table command[NUMCMD] = {
    {"echo", echo, NULL, 0},
    {"man", man, NULL, 1},
    {"initialize", initialize, NULL, 2},
    {"cpulog", cpu, NULL, 3},
    {"plot.2d", plot_2d, NULL, 4},
    {"option", option, NULL, 5},
    {"method", method, NULL, 6},
    {"diffuse", diffuse, NULL, 7},
#ifdef NEWIMPLANT
    {"implant", newimplant, NULL, 8},
    {"select", sel_var, NULL, 9},
#else
    {"implant", implant, NULL, 8},
    {"select", sel_var, NULL, 9},
#endif
    {"contour", contour, NULL, 10},
    {"plot.1d", plot_1d, NULL, 11},
    {"print.1d", print_1d, NULL, 12},
    {"vacancy", vacancy, NULL, 13},
    {"interstitial", interstitial, NULL, 14},
    {"antimony", antimony, NULL, 15},
    {"arsenic", arsenic, NULL, 16},
    {"boron", boron, NULL, 17},
    {"oxide", coeffox, NULL, 18},
    {"phosphorus", phosphorus, NULL, 19},
    {"region", rregion, NULL, 20},
    {"boundary", redge, NULL, 21},
    {"line", rline, NULL, 22},
    {"structure", structure, NULL, 23},
    {"deposit", user_deposit, NULL, 24},
    {"etch", user_etch, NULL, 25},
    {"gold", gold, NULL, 26},
    {"stress", stress_analysis, NULL, 27},
    {"material", c_mater, NULL, 28},
    {"cesium", cesium, NULL, 29},
    {"pause", uPause, NULL, 30},
    {"trap", trap, NULL,	31},
    {"printf", user_print, NULL, 32},
    {"profile", profile, NULL, 33},
    {"mode", mode_cmd, NULL,	34},
    {"device", device, NULL, 35},
    {"beryllium", beryllium, NULL, 36},
    {"carbon", carbon, NULL, 37},
    {"germanium", germanium, NULL, 38},
    {"magnesium", magnesium, NULL, 39},
    {"selenium", selenium, NULL, 40},
    {"isilicon", silicon, NULL, 41},
    {"tin", tin, NULL, 42},
    {"zinc", zinc, NULL, 43},
    {"generic", generic, NULL, 44},
    {"dislocation", dislocation, NULL, 45},
    {"", NULL, NULL, 46},
    {"", NULL, NULL, 47},
    {"", NULL, NULL, 48},
    {"", NULL, NULL, 49}
};

int main(int argc, char *argv[])
{
    int i;
    char dot_name[80], *t;

    /*line at a time buffering*/

    /*initialize the file pointers, which are defined on the compile line*/
    if (t = (char *)getenv("SUP4MANDIR")) 
	MANDIR = t;
    else
	MANDIR = MANLOC;

    if (t = (char *)getenv("SUP4KEYFILE")) 
	KEYFILE = t;
    else
	KEYFILE = KEYLOC;

    if (t = (char *)getenv("SUP4MODELRC")) 
	MODELFILE = t;
    else
	MODELFILE = MODELLOC;

    if (t = (char *)getenv("SUP4IMPDATA")) 
	IMP_FILE = t;
    else
#ifdef NEWIMPLANT
	IMP_FILE = NEWIMPLOC;
#else
	IMP_FILE = IMPLOC;
#endif
    /* 
     *  By default, we use the SUPREM IV gridder
     */
    S7Options.gridder = SUPREM_GRIDDER;
    S7Options.implant_grid_adapt = 0;
    S7Options.diffuse_grid_adapt = 0;
    S7Options.oxidation_grid_adapt = 0;

    /* Parser initialization (verbose is in global.h, not in the parser) */
    strcpy( VersionString, "SUPREM-IV.GS 9450");
    fprintf(stdout, "%s\n", VersionString);
    fprintf(stdout, "\t(c) 1994 Stanford University\n");
    fprintf(stdout, "\t(c) 1991-1993 Stanford University\n");
    fprintf(stdout, "\t(c) 1991 University of Florida\n");
    fprintf(stdout, "\t(c) 2016 Argonne National Laboratory - Modernized!!\n");
    fprintf(stdout, "\t> X11 doesn't work!! Avoid using plot.1d and plot.2d\n");
    fprintf(stdout, "\t> Use strread instead!!\n");
    fprintf(stdout, "Reading Models...\n");
    parser_boot( KEYFILE, "SUPREM4GS> ");
    verbose = V_CHAT;

    /*initialize the diffusion co_efficients and routine pointers*/
    diffuse_init();
#ifdef DEVICE
    device_init();
#endif
    vxmin = vymin = 0.0;
    vxmax = vymax = 1.0;

    /*plot initialization*/
    xgPlotInit();

    /*some grid initialization*/
    rect_boot();

    /*plot initialization*/
    title = salloc(char, strlen(VERSION)+1);
    strcpy(title, VERSION);

    /*ready to start reading input files!*/

    /*read the modelrc file*/
    do_source(MODELFILE, NULL, FALSE, /*report errors*/ TRUE);

    /*read the .supremrc file*/
    strcpy(dot_name, (char *)getenv("HOME")); strcat(dot_name, "/.supremrc");
    do_source(dot_name, NULL, FALSE, /*report errors*/ FALSE);
    do_source(".supremrc", NULL, FALSE, /*report errors*/ FALSE);

    /*do any command line files*/
    for(i = 1; i < argc; i++) {
        do_source(argv[i], NULL, FALSE, /*report errors*/ TRUE );
    }

    /*main command parsing loop*/
    if ( argc < 2 )
        while ( (i = yyparse()) != -1) check_x();

    /*if we are logging cpu stats, close the file*/
    if (cpufile != NULL)
        fclose(cpufile);

    /* xgUpdate(1); */

    return 0;
}






/************************************************************************
 *									*
 *	mode( par, param ) - This card controls the computation mode.	*
 *  It gets its card so that it doesn't get used by mistake.		*
 *									*
 *  Original:	MEL	10/84						*
 *									*
 ************************************************************************/
void mode_cmd(char *par, par_str* param)
{
#   define CHOSEN(x) (is_specified( param, x) && get_bool( param, x))

    /*how much barfola the user wants*/
    if( CHOSEN("one.dim")) {
	set_dim( 1 );
    }
    if( CHOSEN("two.dim")) {
        set_dim( 2 );
    }
}

