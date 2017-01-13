/***********************************************************************/
/**                                                                   **/
/**  Copyright 1994                                                   **/
/**  by The Board of Trustees of Leland Stanford Junior University.   **/
/**                                                                   **/
/**  This software may not be used without the prior written consent  **/
/**  of Stanford University.                                          **/
/**                                                                   **/
/***********************************************************************/

/*   implant.h                Version 5.1     */
/*   Last Modification : 7/3/91 08:55:07 */

#ifndef NewImplantH
#define NewImplantH

#ifdef STATIC_ALLOCATION_TIME
#define EXTERN
#else
#define EXTERN extern
#endif

/* define ions which are not also impurities */
#define BF2 	-1

/* function protos */
EXTERN void implant( char *par, struct par_str* param );

#endif
