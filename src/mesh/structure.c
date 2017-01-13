/*************************************************************************
 *									 *
 *     Copyright c 1984 The board of trustees of the Leland Stanford 	 *
 *                      Junior University. All rights reserved.		 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of Stanford University. *
 *									 *
 *									 *
 *     Copyright c 1989 The board of regents of the University of 	 *
 *                      Florida.  All rights reserved.			 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of the University of 	 *
 *     Florida.								 *
 *									 *
 *************************************************************************/
/*   structure.c                Version 5.1     */
/*   Last Modification : 7/3/91 08:30:57 */

#include <stdio.h>

#ifdef CONVEX
#include <strings.h>
#else
#include <string.h>
#endif

#include <math.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/times.h>

#include "global.h"
#include "constant.h"
#include "impurity.h"
#include "dbaccess.h"
#include "dbase.h"
#include "misc.h"
#include "shell.h"
#include "imagetool.h"
#include "forest.h"

//void ForestOut(char *, char *);


/************************************************************************
 *									*
 *	This stuff is as old as the hills.  Some of this code can be 	*
 *  traced back to the days of the Creig Price and Soo-Young Oh.	*
 *  There have to been to many hacks and changes to mention all the     *
 *  contributors.							*
 *									*
 ************************************************************************/


/************************************************************************
 *									*
 *	structure( par, param ) - This baby reads/writes the current 	*
 *  mesh and solution data.						*
 *									*
 *  Original :	MEL	Apr, 1986					*
 *									*
 *  Added card for saving PIF ASCII files                               *
 *  Goodwin Chin        February, 1989                                  *
 *  Added card for saving Imagetool type files				*
 *  Goodwin Chin	May, 1989					*
 *  Added card for saving SIMPL-2 files					*
 *  Goodwin Chin	November, 1989					*
 *  Added parameter for etch command for Kwang Uhm                      *
 *  Goodwin Chin        February, 1990                                  *
 *  Added parameter for skel files for Kwang Uhm                      	*
 *  Goodwin Chin        May, 1990                                  	*
 *  Added parameter for string etch files for SAMPLE link		*
 *  Goodwin Chin	October, 1990					*
 ************************************************************************/
void structure(
char *par,
struct par_str* param)
{
    int ier;
    struct tms before, after;
    int lflip;
    int i;
    int increment;
    static int filecount;  /* must be static since it keep track of movie
                            *   frame number and we don't want it cleared
                            *   everytime this subroutine exits
                            */
    float scale;
    float backside;
    int skip;
    char *infile;
    char *outfile;
    char *pifile;
    char *imagefile;
    char *simplfile;
    char *headerfile;
    char *etchfile;
    char *skelfile;
    char *piffile;
    char *instr;
    char *framename;
    char ext[4];

    int forest_specified = 0;
    
    /*get the type of mesh to be read, and the filename*/
    infile = get_string( param, "infile" );
    outfile = get_string( param, "outfile" );
    pifile = get_string( param, "pisces" );
    imagefile = get_string( param, "imagetool" );
    simplfile = get_string( param, "simpl" );
    headerfile = get_string( param, "header" );
    etchfile = get_string( param, "etch");
    skelfile = get_string( param, "skel" );
    piffile = get_string( param, "pif" );
    framename = get_string( param, "frame" );

    /* See if we clear counter or if we are incrementing files */ 
    if (get_bool(param, "clear"))
	filecount = 0;

    increment = get_bool(param, "count");

    forest_specified = get_bool(param, "forest");

    ext[3] =  '\0';
    if (increment)  
      {
       sprintf(ext, "%3d", filecount);
       filecount++;
       
       /* pad ext with 0's */
       for (i = 0; i < 3; i++)
	 if (ext[i] == ' ') 
	   ext[i] = '0';
       
       if (framename)
	 {
	  newcat(&framename, ext);
	 }
       else
	 {
	  /* since I add file extensions here we need to allocate more space */
	  if (outfile != NULL)
	    newcat(&outfile, ext);
	  
	  if (pifile != NULL)
	    newcat(&pifile, ext);
       
	  if (imagefile != NULL)
	    newcat(&imagefile, ext);
	 }
      }
    
    /* See if the mesh modification parameters are given */
    lflip = get_bool(param, "flip.y");
    scale = get_float( param, "scale");
   

    /*Are we going in or out? */
    if ( infile ) 
      {
       if (forest_specified)
	 {
	  if (!framename)
	    {
	     printf("Forest input requires a framename als...\n");
	     return;
	    }
	  
/* 	  ForestIn(infile, framename); */
	 }
       else
	 {
	  /* 
	   *  Read HDFVset dude
	   */
	  if (framename)
	    {
/*	     if ((ier = supVGeMeshRead(infile, framename) < 0)) return; */

	     create_db( (nn==0) );
	    }
	  else
	    {
	     if ((ier = ig2_read(infile, lflip, scale) < 0)) return;
	     
	     times(&before);
	     create_db( (nn==0) );
	     times(&after);
	     print_time("connectivity setup", (int *) &before, (int *) &after);
	    }
	 }
      }

    /* write out the file if that is asked for */
    if (outfile) 
      {
       if (framename)
	 {
	  if ( S7Options.gridder == FOREST_GRIDDER)
	    {
/*	     ForestOut(outfile, framename);  */
	     return;
	    }
	  else
	    {
/*	     if ((ier = supVGeMeshWrite(outfile, framename) < 0)) */
	       return;
	    }
	 }
       else
	 {
	  if ( S7Options.gridder == FOREST_GRIDDER )
	    {
	     Forest2Diffuse();
	     create_db(0);
	    }
	  
	  if ((ier = ig2_write(outfile, lflip, scale)) < 0) 
	    {
	     if (S7Options.gridder == FOREST_GRIDDER)
	       dis_all();
	     return;
	    }
	  
	  if (S7Options.gridder == FOREST_GRIDDER)
	    dis_all();
	 }
      }
    

    /* etcetc */
    if (pifile) {
	int show = get_bool( param, "show");
        backside = get_float( param, "backside.y" );
        if (backside > 0.0)
	  if ((ier = newbackside(backside)) < 0) return;
	
        if ((ier = pi_write( pifile, show)) < 0) return;
	if (backside > 0.0) {
	    instr = "structure inf=suprem4temp.str";
	    do_string(instr, "/dev/null", 0);
	}
    }

    if (imagefile)  {
        if ((ier = image_write( imagefile, par, param )) < 0) return;
    }

    if (simplfile)
      if ((ier = simpl_write( simplfile, headerfile )) < 0) return;
    

    if (etchfile)  {
        skip = get_int( param, "line" );
        if ((ier = polygon_etch(etchfile, skip)) < 0) return;
	
    }

    if (skelfile) 
      if ((ier = skel_write(skelfile, lflip, scale)) < 0) return;
    

    if (piffile)
      if ((ier = pif_write(piffile)) < 0) return;
    


    /*was this a reflection command?*/
    if( get_bool( param, "mirror")) {
	float sign=1; int xy=0;
	if( get_bool( param, "left")) sign = -1;
	if( is_specified( param, "top") && get_bool( param, "top")) {xy=1;sign= -1;}
	if( is_specified( param, "bottom") && get_bool( param, "bottom")) {
	    xy=1; sign= 1;}
	reflect_grid( sign, xy);
    }
    return;
    
}


void newcat(
char **a,
char *b)
{
    *a = sralloc(char, strlen(*a) + strlen(b) + 1, *a);
    strcat(*a,b);
}

int skel_write( 
     char *str, int i1, float f1)
{
 return(0);
}

int pif_write( 
	      char *str) 
{ 
 return(0);
}

int polygon_etch( 
char *str, int foo) 
{ 
return(0);}


