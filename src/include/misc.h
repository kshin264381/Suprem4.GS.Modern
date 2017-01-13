/*************************************************************************
 *									 *
 *     Copyright c 1993 The board of regents of the University of 	 *
 *                      Florida.  All rights reserved.			 *
 *									 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   misc.h                Version 5.1     */
/*   Last Modification : 5/11/93  10:38:29 */

#ifndef MiscH
#define MiscH

#include <unistd.h>

//extern struct tms;
//extern struct par_str;
//extern struct bool_exp;
//extern union val_str;

//struct tms;
//struct par_str;
//struct bool_exp;
//union val_str_u;

#include "data_types.h"

/* parser.c?? */
void yyerror(
char *s);
/* parser.c?? */

/* cpu.c */
void cpu(
char *par,
struct par_str* param);
/* cpu.c */

/* echo.c */
void echo(
char *par,
struct par_str* param);

void uPause(char *par, struct par_str* param);

void user_print(char *par, struct par_str* param);
/* echo.c */

/* get.c */
int get_value(par_str **par, char *name, val_str *value);

int get_bool(
struct par_str* par,
char *name);

char *get_string(
struct par_str* par,
char *name);

int get_int(
struct par_str* par,
char *name);

float get_float(
struct par_str* par,
char *name);

int is_specified(
struct par_str* par,
char *name);
/* get.c */

/* man.c */
void man(
char *par,
struct par_str* param);
/* man.c */

/* pa.c */
void
par(int first ,int last );

void
pa(double *dp , int first, int last);
/* pa.c */

/* panic.c */
void panic (char *s);
void GridSave ();
/* panic.c */

/* print_time.c */
void print_time(
char *name,
int *before, int *after);
/* print_time.c */

/* read.c */
int read_ukfile(
char *name);
/* read.c */

/* reader.c */
int read_list(
struct par_str **par,
FILE *fd);

int read_param(
struct par_str *par,
FILE *fd);

int read_boolean(
struct bool_exp **bexp,
FILE *fd);
/* reader.c */

/* sint.c */
float sol_interp(
int type,
float val1,
float val2);

float interface(
int type,
float loc);
/* sint.c */

/* grid_loop.c */
//double fmin(double x, double y);
//double fmax(double x, double y);
/* grid_loop.c */


#endif
