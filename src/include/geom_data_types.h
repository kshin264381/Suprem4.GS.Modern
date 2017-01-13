/*
 * geom_data_types.h
 *
 *  Created on: Mar 30, 2016
 *      Author: kshin
 */

#ifndef SRC_INCLUDE_GEOM_DATA_TYPES_H_
#define SRC_INCLUDE_GEOM_DATA_TYPES_H_

#include "constant.h"

/*a structure to hold connectivity*/
struct list_str {
    unsigned int num;		/*the number in the list*/
    unsigned int all;		/*the amount allocated*/
    int *list;		/*the number in the list*/
};

/*a structure to hold connectivity*/
struct box_str {
    unsigned int num;		/*the number in the list*/
    unsigned int all;		/*the amount allocated*/
    float *list[MAXDIM];	/*the coordinates of each point in the list*/
};

/*define a structure of the physical point information*/
struct pt_str {
    float cord[MAXDIM];	      /* the coordinate value of the point */
    float cordo[MAXDIM];      /* the initial coordinates of the point */
    float vel[MAXDIM];	      /* the current velocity */
    float ovel[MAXDIM];	      /* the last velocity */
    float spac;		      /* local spacing */
    int flags;		      /* set of bits to indicate special conditions */
    int nn;		      /* the number of nodes at this point */
    int nd[MAXMAT];	      /* indices into the node structure */
    void *pflag;              /* an undefined pointer, will be used to point
				 to the corresponding Forest Point or other */
};
typedef struct pt_str pt_typ;

/*define a structure for the logical nodes*/
struct nd_str {
    double sol[MAXIMP];		/*the solution values for each stream*/
    int mater;			/*the material index for this node*/
    double step;		/*the step the node was added at*/
    double time;		/*the time of the step it was added*/
    struct list_str ltri;	/*the list of triangles*/
    struct list_str edg;	/*the list of edges*/
    int pt;			/*the index of the physical point*/
    int flags;			/*set of bits to indicate special conditions*/
};
typedef struct nd_str nd_typ;


/*define the structure to contain the element information*/
struct tri_str {
    int nd[MAXVRT];		/*the index to the nodes in the element*/
    int nb[MAXSID];		/*the neighbor element*/
    int edg[MAXSID];		/*the edges for each triangle*/
    float ehed[MAXSID];	/*the coupling coefficients*/
    float d[MAXSID];		/*the length of the sides - boundary conditions*/
    float earea[MAXVRT];	/*the area of the element for each node*/
    float sig[MAXVRT];	/*the per-element stresses*/
    int regnum;		/*the region number of the element*/
    int fath;		/*the father of this element*/
    int son;		/*the first son of this element*/
    int level;		/*the height in the tree (redundant but fun)*/
    int flags;		/*set of bits to indicate special conditions*/
};
typedef struct tri_str tri_typ;

/*define a structure for region data*/
struct edg_str {
    int nd[2];		/*nodes at ends of the edge*/
    struct list_str ele;		/*element list*/
    struct list_str skel;		/*skeleton list*/
    int flags;		/*data base flags*/
    float cpl;		/*the coupling coefficient along the edge*/
    float len;		/*the length of the edge*/
    int u, l;		/*the location of lower and upper couplings*/
};
typedef struct edg_str edg_typ;

/*define a structure for region data*/
struct reg_str {
    int mater;		/*material type of region*/
    int flags;		/*data base flags*/
    double thick;	/*thickness of deposited layer*/
    double density;	/*density of layer material*/
    struct list_str edg;
    struct list_str tri;
    struct list_str fac;
    struct list_str nd;
    int sreg;		/*the skeleton index*/
    };
typedef struct reg_str reg_typ;

#endif /* SRC_INCLUDE_GEOM_DATA_TYPES_H_ */
