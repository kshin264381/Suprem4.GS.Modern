/*   movie.c                Version 1.7     */
/*   Last Modification : 7/21/94 15:51:23        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
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
 *************************************************************************/

#include <stdio.h>
#include <math.h>
#include "constant.h"
#include "global.h"
#include "shell.h"
#include "plot.h"
#include "misc.h"

/************************************************************************
 *									*
 *	make_movie( string, time ) - This routine executes the commands *
 *  in the string passed.  The time elapses is in time.			*
 *									*
 *  Original:	MEL	11/86						*
 *									*
 ************************************************************************/
void make_movie(
char *commands,		/*the commands*/
float time)		/*the length of time to take this diffusion/oxidation*/
{
    char buffer[512];

    if ( commands == NULL ) return;

    /*define a macro string for the current time*/
    if ( time < 1.0 ) {
	sprintf(buffer, "tlab %7.3fms", time * 1000.0 );
	define_macro( buffer, &macro );
    }
    else if ( time < 60.0 ) {
	sprintf(buffer, "tlab %7.3fs", time );
	define_macro( buffer, &macro );
    }
    else if ( time < 3600.0 ) {
	sprintf(buffer, "tlab %7.3fm", time / 60.0 );
	define_macro( buffer, &macro );
    }
    else {
	sprintf(buffer, "tlab %7.3fh", time / 3600.0 );
	define_macro( buffer, &macro );
    }

    sprintf(buffer, "time %g", time);
    define_macro( buffer, &macro );

    /*make it fancy later with the time, but right now just execute*/
    do_string( commands, NULL, 0 );

    /*undo the macro*/
    (void)undef_macro( "time", &macro );
    (void)undef_macro( "tlab", &macro );
    check_x();
}
