/*   diffuse.c                Version 1.53     */
/*   Last Modification : 12/6/94 21:32:39        */
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
 *     Copyright c 1991 The board of regents of the University of 	 *
 *                      Florida.  All rights reserved.			 *
 *     This subroutine may not be used outside of the SUPREM4 computer	 *
 *     program without the prior written consent of the University of 	 *
 *     Florida.								 *
 *									 *
 *									 *
 *************************************************************************/
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/times.h>

#include "global.h"
#include "constant.h"
#include "geom.h"
#include "diffuse.h"
#include "defect.h"
#include "matrix.h"
#include "impurity.h"
#include "material.h"
#include "expr.h"
#include "misc.h"
#include "dbase.h"
#include "oxide.h"
#include "forest.h"
#include "dislocation.h"

extern void flushbuf(void);

/************************************************************************
 *									*
 *	diffuse( par, param ) - diffuse solves the partial differential *
 *  equations associated with the diffusion process.     		*
 *									*
 *  Original:	MEL	1/85						*
 *									*
 ************************************************************************/
void diffuse( char *par, struct par_str* param)
{ 
 float time, temp, cs;
 struct tms before, after;
 int cont;
 int dump;
 int r;
 char *movie;
 double t;			/*the current time, total*/
 static double del_df;	/*the next delta for the diffusion*/
 double last_ddf;		/* the last delta for the diffusion */
 double Ktd, Kto, Ktp, Ktdisloop;

 int wrcnt = 1;
 register int i;
 double err = 1.0;
 double *oarea;
 int firsttime = 1;
 int adapting;
 int gridaltered = 0;
 int last_step;
 FILE *f1,*f2,*f3;
 int regrid;
 int adaptgrid;
 struct tms timebuf;
 clock_t howlong;
 double next_ddf;

#define LAW_DIFFUSION 0
#define YERGEAU_DIFFUSION 1
 int diffusioncode = LAW_DIFFUSION;
 
 time = get_float( param, "time" ) * 60;		/*convert to seconds*/
 last_temp = temp = get_float( param, "temp" ) + 273; 	/*convert to Kelvin*/
 cont = is_specified( param, "cont" ) && get_bool( param, "cont" );
 if (get_bool (param, "argon")) gas_type = GAS_ARGON;
 if (get_bool (param, "nitrogen")) gas_type = GAS_ARGON;
 if (get_bool (param, "ammonia")) gas_type = GAS_ARGON;
 if (get_bool (param, "dryo2")) gas_type = GAS_DRYO2;
 if (get_bool (param, "weto2")) gas_type = GAS_WETO2;
 if (get_bool (param, "antimony")) gas_type = GAS_Sb;
 if (get_bool (param, "arsenic")) gas_type = GAS_As;
 if (get_bool (param, "boron")) gas_type = GAS_B;
 if (get_bool (param, "phosphorus")) gas_type = GAS_P;
 if (get_bool (param, "beryllium")) gas_type = GAS_Be;
 if (get_bool (param, "magnesium")) gas_type = GAS_Mg;
 if (get_bool (param, "selenium")) gas_type = GAS_Se;
 if (get_bool (param, "isilicon")) gas_type = GAS_Si;
 if (get_bool (param, "tin")) gas_type = GAS_Sn;
 if (get_bool (param, "germanium")) gas_type = GAS_Ge;
 if (get_bool (param, "zinc")) gas_type = GAS_Zn;
 if (get_bool (param, "carbon")) gas_type = GAS_C;
 if (get_bool (param, "generic")) gas_type = GAS_G;
 
 if (is_specified( param, "solid.sol")) 
   gas_conc = -1.0;
 else
   gas_conc = get_float( param, "gas.conc" );
 pressure = get_float( param, "pressure");
 
 dump = get_int (param, "dump");
 movie = get_string( param, "movie" );

 if (get_bool (param, "adapt"))
   adaptgrid = 1;
 else
   adaptgrid = 0;

 if (get_bool (param, "yergeau"))
   diffusioncode = YERGEAU_DIFFUSION;

 /*change the solution flags from the default configuration*/
 CLEAR_FLAGS(Psi, ALL_FLAGS);
 SET_FLAGS(Psi, (PSEUDO | MOBILE | DIFFUSING | STEADY | LOCKSTEP) );

 if (!cont) 
   total = 0.0;
 else 
   methdata.init_time = 0.0;


 times(&before);

 /*set up the loop*/
 t = 0;

 if ( methdata.init_time != 0.0 ) 
   del_df = methdata.init_time;
 last_ddf = del_df;

 last_step = 0;
 
 howlong = times(&timebuf);

 printf("before = %ld \n", (long) timebuf.tms_utime );    

 times(&before);

 /*loop until t = the time at the end of the ride*/
 while ( t < time ) 
   {
    regrid = 0;
    
    if ( (t + del_df) > time) 
      {
       del_df = time - t;
       last_step = 1;
      }

    /*Print some sort of header for the timestep*/
    if (verbose >= V_NORMAL)
      printf("--------------------\nelapsed time = %g\n", total);
    else
      printf("Solving %12g + %12g = %12g, %8g%%, np %d\n",
	     total,del_df, total+del_df, 100*del_df/last_ddf, np);
    
    
    if (( S7Options.gridder == FOREST_GRIDDER ) && 
	( diffusioncode == LAW_DIFFUSION))
      {
       Forest2Diffuse();
       create_db(0);
      }
    else if ((S7Options.gridder == FOREST_GRIDDER) &&
	     (diffusioncode == YERGEAU_DIFFUSION))
      {
       /* Forest2Yergeau(); */

       if (firsttime)
	 {
	  /* Yergeau - first time step stuff */
	 }
      }
    
    if ((diffusioncode == LAW_DIFFUSION) || (oxidizing))
      {
       if ((firsttime) || (gridaltered))
	 {
	  check_active();
	  
	  if (!oxidizing)
	    {
	     lose_impurity( O2);	lose_impurity( H2O);
	     lose_impurity( Sxx);	lose_impurity( Sxy);	
	     lose_impurity( Syy);
	     
	     /*check for polysilicon*/
	     for(r = 0; (r < nreg) && (mat_reg(r) != Poly); r++);
	     
	     /*no poly silicon*/
	     if ( r == nreg ) 
	       {
		lose_impurity( XVEL);   
		lose_impurity( YVEL);   lose_impurity( DELA);
	       }
	    }
	  else
	    {
	     if (!cont)
	       {
		/* Reset the 1d thickness. */
		t1d = tinit;

		if (S7Options.gridder == SUPREM_GRIDDER)
		  {
		   /* Add a thin oxide layer to any exposed silicon surfaces */
		   NativeOxide(tinit);
		   geom("after native oxide deposit");
		  }
	       }
	    }
	  
	  if ( is_specified( param, "gold.surf" ) ) 
	    {
	     cs = get_float( param, "gold.surf" );
	     add_impurity( Au, cs, GAS);
	    }
	  
	  predep();

	  /*initialize temperature dependent material stuff*/
	  comp_mat( temp );
	  
	  /*initialize defects to equilibrium values as a function of temp*/
	  init_pseudo( temp );
	  
	  /*initialize the diffusion code*/
	  init_diffuse();
	  
	  UpdateSymbolic = 0;
	  
	  /*initialize the equilibrium defect concentrations*/
	  err = start_step( temp, cont );
	  printf("estimated first time step %e\n", err);
	  
	  /*the time dependent writes*/
/*	  dump_data( total, dump, movie, &wrcnt ); */
	 }
       
       if ((!firsttime) && (!gridaltered))
	 {
	  UpdateSymbolic++;
	 }
      } /* if we are using Mark Law's diffusion solver */
    

    /*The oxidation step.  It may cut back the time step */
    if (oxidizing) 
      {
       if (diffusioncode != LAW_DIFFUSION)
	 {
	  if (S7Options.gridder == FOREST_GRIDDER)
	    {
	     Forest2Diffuse();
	     create_db(0);
	    }
	 }
       
       if (S7Options.gridder != FOREST_GRIDDER)
	 {
	  GridSave();
	 }

       do_oxide(temp,sub_ornt,(gas_type==GAS_WETO2), &del_df);
       
       if (S7Options.gridder == FOREST_GRIDDER)
	 {
	  if (!firsttime)
	    del_df = next_ddf;
	  
	  Diffuse2Forest(0);
	  del_df = Kto = ForestGridDt( del_df );
	 }
       else if (S7Options.gridder == SUPREM_GRIDDER)
	 {
	  Kto = grid_dt();
	 }
       else
	 Kto = del_df;
       
       if (Kto < 0.05)
	 regrid = 1;
      }
    else
      Kto = time - t + 1;
    
    if (diffusioncode == LAW_DIFFUSION)
      {
       /*solve the time step and get the increase*/
       /* Choose the more conservative of the oxide and diffusion increases */
       Ktd = solstep( del_df, temp, methdata.timemeth, TRUE );
       if (loopr > 1.0e-7 ) {
           Ktdisloop = do_disloop(temp, &del_df);
       }
       else {
           Ktdisloop = MAXFLOAT;
       }

       if ( S7Options.gridder == FOREST_GRIDDER )
	 {
	  Diffuse2Forest(1);
	 }
      }
    else if (diffusioncode == YERGEAU_DIFFUSION)
      {
       /* Ktd = YergeauSolveStep( del_df, temp ); */       
       /* del_df is the max time step from grid crashing in oxidation */
       /* temp is the temperature */
       /* Ktd is the time step that the diffusion code will allow */
      }
    
    if (oxidizing)
      {
       if (S7Options.gridder == FOREST_GRIDDER)
	 {
	  gridaltered = ForestMoveNodes(del_df, regrid);
	 }
      }

    if (!last_step)
      {
       if (adaptgrid)
	 gridaltered = GridAdapt(1, 1);
      }

    total += del_df;
    t += del_df;

    dump_data( total, dump, movie, &wrcnt ); 

    last_ddf = del_df;
    /* take the most conservative time step */
    del_df = min(Ktd, min(Kto,Ktdisloop));
/*    del_df = min(Ktd, Kto); */

/*    del_df = Ktd;*/		/* diffusion will always be smaller */

    next_ddf = min(Ktd, Ktdisloop);
    
    /* But if given a formula, take that instead */
    if( methdata.timemeth == FORM)
      del_df = time_form( methdata.dtform, total, del_df);
    
    if( verbose >= V_NORMAL)
      printf("    new timestep is %g increase %g [Ktd %.2e Kto %.2e]\n", 
	     del_df, del_df/last_ddf, Ktd/last_ddf, Kto/last_ddf);

    if (oxidizing)
      {
       if (S7Options.gridder == SUPREM_GRIDDER)
	 {
	  GridSave();
	  grid_sub( last_ddf );
	 }
      }

    times(&after);
    print_time("Time for step", (int *) &before, (int *) &after);
    
    /* Flush those buffers, you sucker! */
    flushbuf();
    
    firsttime = 0;
   } /* end of while loop */

 howlong = times(&timebuf);
 printf("before = %ld \n", (long) timebuf.tms_utime );    
 
 times(&after);
 print_time("total diffusion time", (int *) &before, (int *) &after);
 damage_read=FALSE;
}



