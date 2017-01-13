/*************************************************************************
 *                                                                       *
 *     Copyright c 1991 The board of regents of the University of        *
 *                      Florida.  All rights reserved.                   *
 *     This subroutine may not be used outside of the SUPREM4 computer   *
 *     program without the prior written consent of the University of    *
 *     Florida.                                                          *
 *                                                                       *
 *                                                                       *
 *************************************************************************/
/*   dbflags.h                Version 5.1     */
/*   Last Modification : 7/3/91 08:54:59  - used to be in dbaccess.h */

#ifndef DbflagsH
#define DbflagsH

/************************************************************************
 *                                                                      *
 *      This file contains flags that are used by the data base         *
 *  access.                                                             *
 *                                                                      *
 ************************************************************************/

/*big useful macros*/
#define set(A,B)        A->flags |= B
#define clr(A,B)        A->flags &= ~B
#define ask(A,B)        (A->flags & B)

/*global dirty variables*/
EXTERN int bc_dirty;		/*boundary codes for triangles*/
EXTERN int tripts;		/*some tris point to points*/
EXTERN int geom_dirty;
EXTERN int clkws_dirty;
EXTERN int nd2tri_dirty;
EXTERN int neigh_dirty;
EXTERN int edge_dirty;
EXTERN int nd2edg_dirty;
EXTERN int reg_dirty;
EXTERN int need_waste;

/*data base flag for everybody*/
#define KILL_ME     0x8000
#define KILL_LATER  0x4000

/*Data Base Flags for nodes*/
#define ND2EDG	0x0001
#define ND2TRI	0x0002
#define REG_BND	0x0004

/*Data Base Flags for points*/
#define SURFACE 0x0001
#define BACKSID 0x0002
#define SKELP 0x0004

/*Data Base Flags for triangles*/
#define TRIPTS	0x0001
#define GEOMDN	0x0002
#define CLKWS 	0x0004
#define NEIGHB  0x0008
#define CHPFIX  0x0010

/*Data Base Flags for edges*/
#define REGS	0x0001		/*edge points to skeleton regions*/
/*use geomdn for triangles*/
#define ESURF   0x0004
#define EBACK   0x0008
#define MARKED  0x0010

/*Data Base Flags for regions*/
#define EDG	0x0001
#define EXPOS   0x0002	/*is this region exposed to the surface*/
#define SKEL	0x0004	/* this is a skeleton region, unfilled*/
#define ETCHED  0x0008	/* don't etch regions multiple times*/

/*the code to represent non-existant sons or fathers in the element tree*/
#define NO_TRIANGLE -1

/*the code used by element to indicate a neumann edge*/
#define BC_OFFSET -1024
#define EXPOSED BC_OFFSET+2
#define BACKEDG BC_OFFSET+1


#endif
