/*
 * data_types.h
 *
 *  Created on: Mar 30, 2016
 *      Author: kshin
 */

#ifndef SRC_INCLUDE_TYPES_H_
#define SRC_INCLUDE_TYPES_H_

//struct vec_str;
//struct tok_str;

/* define a type for the data in a table field */
typedef union uval_str {
    int ival;
    float dval;
    char *sval;
    } val_str;

/* define the boolean expression structure */
typedef struct bool_exp {
    val_str value;		/*the value at this node*/
    int type;			/*the type of value that was*/
    struct bool_exp *right;	/*the right hand son*/
    struct bool_exp *left;	/*the left hand son*/
    } bool_exp;

/* structure to store away an individual parameter */
typedef struct par_str {
    char name[12];		/* the parameter name                        */
    char *units;		/* the units to specify the parameter in     */
    val_str def;		/* the parameter default value               */
    val_str value;		/* the parameter value                       */
    struct bool_exp *bexp;	/* the parameter legality function           */
    int type;			/* type of the parameter                     */
    char *err_msg;		/* pointer to the error message at this node */
    struct par_str **param;	/* list of sub parameters                    */
} par_str;

/*structure for the parameters to be passed to the boundary code routine*/
typedef struct bound_str {
    int nx[2];          /*the node numbers involved*/
    double delta;       /*timestep*/
    float temp;         /*temperature*/
    double vel[2];      /*boundary velocity*/
    double conc[2];     /*solution values*/
    double eq[2];       /*equilibrium values*/
    int mat[2];         /*material values*/
    int loc[2][2];      /*coupling locations*/
    double cpl;         /*coupling length*/
    double **rhs;       /*right hand side*/
    float cord[2];      /*location of the nodes*/
    double vmax;        /*maximum velocity*/
    double dela[2];     /*change in the area*/
} bound_str;

/*type def a relationship for the value field*/
typedef union {
    float dval;
    int ival;
    char *sval;
    struct vec_str *bval;
} plt_val;

/*define a streucture to hold everything in*/
typedef struct vec_str {
    plt_val value;
    int type;
    struct vec_str *right;
    struct vec_str *left;
} vec_str;


/*declare a structure for parsing*/
typedef struct tok_str {
    int type;
    plt_val value;
} tok_str;


#endif /* SRC_INCLUDE_TYPES_H_ */
