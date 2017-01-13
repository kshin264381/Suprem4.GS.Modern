/*************************************************************************
 *									 *
 *   Original : MEL         Stanford University        Sept, 1984	 *
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   check.h                Version 5.1     */
/*   Last Modification : 7/3/91  08:54:56 */

 /***********************************************************************
 *									*
 *	check.h - this file contains common definitions for the 	*
 *  parameter parser.  							*
 *									*
 ***********************************************************************/

#ifndef CheckH
#define CheckH

#include "data_types.h"

//extern struct vec_str;
//extern struct tok_str;

//struct vec_str;
//struct tok_str;

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

#define NUMPAR 100	/* the maximum number of parameters for a command */

/* define set values for data types, also used for interactive routines */
/* flags defined here to be used in type var as well */
#define INDEX 0x003F
#define REAL 0x0040
#define INT  0x0080
#define STR  0x0100
#define BOOL 0x0200
#define COMM 0x0400
#define CHOICE 0x1000
#define SPECIFIED 0x0800
#define MASK 0x083F

/* define types used in the boolean trees */
#define OPER 1
#define CONSTREAL 2
#define PARVAL 3

///* define a type for the data in a table field */
//typedef union uval_str {
//    int ival;
//    float dval;
//    char *sval;
//    } val_str;
//
///* define the boolean expression structure */
//struct bool_exp {
//    val_str value;		/*the value at this node*/
//    int type;			/*the type of value that was*/
//    struct bool_exp *right;	/*the right hand son*/
//    struct bool_exp *left;	/*the left hand son*/
//    };
//
///* structure to store away an individual parameter */
//par_str {
//    char name[12];		/* the parameter name                        */
//    char *units;		/* the units to specify the parameter in     */
//    val_str def;		/* the parameter default value               */
//    val_str value;		/* the parameter value                       */
//    struct bool_exp *bexp;	/* the parameter legality function           */
//    int type;			/* type of the parameter                     */
//    char *err_msg;		/* pointer to the error message at this node */
//    par_str **param;	/* list of sub parameters                    */
//    };

EXTERN par_str *cards[NUMPAR];

/* forward declarations for functions in the files in the check directory */
val_str bool_check(
#ifdef __cplusplus
     struct bool_exp *bexp,
     int *typ
#endif
);

int error_check(
#ifdef __cplusplus
     par_str **par,
     int boolian
#endif
);

void deflt_check( 
#ifdef __cplusplus
     par_str **par
#endif
);

void assign_deflt(
#ifdef __cplusplus
     par_str **par
#endif
);

char *eval_real(
#ifdef __cplusplus
     struct vec_str *expr,
     float *val
#endif
);

char *eval_vec(
#ifdef __cplusplus
     struct vec_str *expr,
     float *val
#endif
);

int islogexp(
#ifdef __cplusplus
     struct vec_str *expr
#endif
);

void free_expr(
#ifdef __cplusplus
     struct vec_str *t
#endif
);

int single_char(
#ifdef __cplusplus
     char *s,
     struct tok_str *tok
#endif
);

int functions(
#ifdef __cplusplus
     char *str,
     struct tok_str *tok
#endif
);

int lexical(
#ifdef __cplusplus
     char *str,
     struct tok_str *tok
#endif
);

int detect_error();

char *parse_expr( 
#ifdef __cplusplus
     char *str,
     struct vec_str **out
#endif
);

void expr_reduce(
#ifdef __cplusplus
     struct tok_str *a, struct tok_str *b, struct tok_str *c, struct
tok_str *d
#endif
);

int reduce();

//float sol_interp(
//#ifdef __cplusplus
//     int type,
//     float val1,
//     float val2
//#endif
//);
//
//float interface(
//#ifdef __cplusplus
//     int type,
//     float loc
//#endif
//);

int split(
#ifdef __cplusplus
     char *line,	/*the input line to broken*/
     char **argv,	/*the return parameter value pairs*/
     int parnames
#endif
);	/*are there leading parameter names*/

char *get_solval(
#ifdef __cplusplus
     float *val,
     int type
#endif
);

char *vfunc(
#ifdef __cplusplus
     float *val,
     int type,
     struct vec_str *expr
#endif
); /*argument of the vector function*/

int constants(
#ifdef __cplusplus
     char *str,
     struct tok_str *tok
#endif
);

int sol_values(
#ifdef __cplusplus
     char *s,
     struct tok_str *tok
#endif
);

int vec_func(
#ifdef __cplusplus
     char *s,
     struct tok_str *tok
#endif
);

void RoughGrad(
#ifdef __cplusplus
     int which_one,
     float *vector
#endif
);

char *parse_real(
#ifdef __cplusplus
     char *str,
     float *dval
#endif
);

int name_check(
#ifdef __cplusplus
     par_str **pars,
     char *name,
     par_str **ret
#endif
);

int verify(
#ifdef __cplusplus
     char *name,
     par_str **pars,
     val_str *value,
     par_str **parloc
#endif
);

int check(
#ifdef __cplusplus
     char *str,
     par_str *par
#endif
);

void unspecify(
#ifdef __cplusplus
     par_str **par
#endif
);


#endif
