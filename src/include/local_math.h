/*************************************************************************
 *									 *
 *     Copyright c 1993 The board of regents of the University of 	 *
 *                      Florida.  All rights reserved.			 *
 *									 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *************************************************************************/
/*   math.h                Version 5.1     */
/*   Last Modification : 5/11/93  10:38:29 */
#ifndef LocalMathH
#define LocalMathH

//EXTERN struct d_sort;
struct d_sort;

typedef struct list_node
{
	struct list_node *n;
	int v;
} link_t;

void blkfac(
	int nv,				/*number of variables per block*/
	int nsol,			/*number of blocks*/
	int *sol,			/*the particular blocks*/
	int *il,			/*l matrix descriptor*/
	double *l,			/*l matrix values*/
	int loff)			/*l upper triangular offset*/;

void app_inv(
	int nv,
	int nsol,
	int *sol,
	int *il,
	int loff,
	double *l,
	double *x[],
	double *ax[]);

void numfac(
	int n,
	int *sol,
	int nsol,
	int ia[],
	int aoff,
	double *a,
	int *il,
	int loff,
	double l[]);

int  symfac(
	int n,
	int *sol,
	int nsol,
	int *ia,
	int aoff,
	int **pil,
	int *loff);

void numbac(
	int n,		/*the number of equations*/
	int il[],	/*the array description of l*/
	int off,	/*the offset to the upper triangular part*/
	double l[],	/*the factored array*/
	double rhs[])	/*the right hand side vector*/;

void dzero(
    double *a,
    int n);

void dcopy(
    double *a,
	double *b,
    int n);

void get_coldata(
	int vi,		/*variable index*/
	int bi,		/*block index*/
	int nsol,	/*number of solution blocks*/
	int *sol,	/*solution blocks*/
	int *ia,	/*the a matrix indeices*/
	int aoff,	/*the upper a offset*/
	double *col,	/*the column variables*/
	double *row)	/*the row variables*/;

int get_locs(
	int vi,		/*variable index*/
	int bi,		/*block index*/
	int nsol,	/*number of solution blocks*/
	int *sol,	/*solution blocks*/
	int *ia,	/*the a matrix indeices*/
	int aoff,	/*the upper a offset*/
	int *icol)	/*the column locations*/;

int generate(
    int nn,			/* Number of nodes */
    int symb_var,		/* Numbers of degrees of freedom per node */
    int symm,			/* Do upper *and* lower halves? */
    int (*get_connect)(int , short *, int *),	/* Returns nodal connections */
    int **pia,			/* Symbolic matrix map */
    int *aoff)			/* Size of upper triangle (redundant)*/;

int iaja(
    int *ia, int aoff,
    int row,
    int col);

void dump_ij(int *ia);

void dump_a(
    int *ia,
    double *a,
    double *rhs);

void genrow(
	int *ia,
	int aoff,
	int *irow,
	int *jrow);

int min_ia_fill(
	int *ia,			/* Matrix map including l and u */
	int *reorder,		/* Result vector */
	int talk);

int o_compar(struct d_sort *f1, struct d_sort *f2);

void new_storage();

void run_length(int nn, link_t *nbrs);

void scramble(int *reorder);

void do_symb(
	int nsol,	/*number of solution variables being worked with*/
	int *sol,	/*the locations*/
	int **il,	/*the il index array for this set of variables*/
	double **l,	/*the l array for this set of variables*/
	int *loff);	/*the offset*/

int get_connect(
	int ind,	/*the node index*/
	short *taken,	/*the result matrix*/
	int *num);	/*the number of neighbors found: initially max.*/

void clean_list(
	short *taken,
	int *num);

double norm2(
	double *arr,
	int n);

void solve_blocks(
	int nv,
	int *sol,
	int nsol,
	int *elim,
	int *il,
	double *l,
	int loff,
	double *wrkspc[],
	int factor);

void mxv(
	int nv,
	int *ia,
	int aoff,
	double sign,
	double *a,
	double *v,
	double *av);

void mxv_tri(
	int nv,
	int *ia,
	int aoff,
	double sign,
	double *a,
	double *v,
	double *av);

void bigmxv(
	int nsol, int *sol, int nv,
	double **vec,	/*the vector*/
	double **avec);	/*a x vector*/

void blkmxv(
	int b1,		/*the block number*/
	int b2,		/*the other block number*/
	int bnot,	/*don't redo this number block*/
	int nv,		/*the number of variables*/
	int sum,	/*sum or not*/
	double *vec,	/*the vector*/
	double *avec);	/*the result*/

void vinit(
	double a[],
	int nv);

void mxv_diag(
	int nv,
	double sign,
	double *a,
	double *v,
	double *av);

/* math/fp_utils.c */
// Floating point comparator with epsilon comparison method
// A == B?
int f_equal(float inputA, float inputB);
// A < B?
int f_lt(float inputA, float inputB);
// A > B?
int f_mt(float inputA, float inputB);

// Double precision comparator with epsilon comparison method
// A == B?
int d_equal(double inputA, double inputB);
// A < B?
int d_lt(double inputA, double inputB);
// A > B?
int d_mt(double inputA, double inputB);
/* math/fp_utils.c */

#endif /* Include guard */
