/*   %M%                Version %I%     */
/*   Last Modification : %G% %U%        */
/*-----------------------------------------------------------------*/
/* Copyright (c) 1993, 1994                                        */ 
/* The Board of Trustees of the Leland Stanford Junior University. */
/* All rights reserved.                                            */
/* Author: Zakir Sahul                                             */
/*-----------------------------------------------------------------*/

#include "global.h"
#include "constant.h"
#include "dbaccess.h"
#include <stdio.h>
#include <ctype.h>
#include "material.h"
#include "impurity.h"
#include "sysdep.h"
#include "expr.h"       /*for last_temp*/
#include "dbase.h"

#include "VGeMesh.h"

int supVGeMeshWrite(char *filename, char *framename)
{
 /* 
  *  First form the arrays and then pass them off to the
  *  real writer
  */

 /* 
  *  First the coordinates.
  *  Note that we multiply SUPREM coordinates by 1e4 to
  *  make them to be in microns and we do a -1 scaling
  *  of the y coordinates to make this a right handed
  *  system
  */
 float *pi = new float[nn];
 float *pj = new float[nn];
 
 for (i = 0; i < nn; i++)
   {
    pi[i] = nd[i]->pt->cord[0] * 1e4;
    pj[i] = -1.0 * nd[i]->pt->cord[1] * 1e4;
   }

 /* 
  *  Count how many exposed triangles there are
  *  This is needed for "Gas" triangles
  */
 for (i = 0, num_exposed = 0; i < ne; i++)
   {
    for (j = 0; j < 3; j++)
      {
       if ((tri[i]->nb[j] == -1023) || (tri[i]->nb[j] == -1024))
	 {
	  num_exposed++;
	 }
      }
   }

 /* 
  *  OK, figure out the connectivity, auxiliary connectivity
  *  and material
  */
 int *tri3 = new int[(ne + num_exposed) * 3];
 int *auxc = new int[(ne + num_exposed) * 3];
 int *matstuff = new int[(ne + num_exposed)];
 int totalele = ne + num_exposed;
 
 num_exposed = 0;
 for (i = 0; i < ne; i++)
   {
    matstuff[i] = reg[tri[i]->reg]->mater;

    for (j = 0; j < 3; j++)
      {
       *(tri3 + (3 * i) + j) = tri[i]->nd[j];
       *(auxc + (3 * i) + j) = tri[i]->nb[j];

       if ((tri[i]->nb[j] == -1023) || (tri[i]->nb[j] == -1024))
	 {
	  *(tri3 + (3 * (ne + num_exposed)) + 0) = tri[i]->nd[(j + 1)%3];
	  *(tri3 + (3 * (ne + num_exposed)) + 1) = tri[i]->nd[(j + 2)%3];
	  *(tri3 + (3 * (ne + num_exposed)) + 2) = -1;

	  *(auxc + (3 * (ne + num_exposed)) + 0) = i;
	  *(auxc + (3 * (ne + num_exposed)) + 1) = -1;
	  *(auxc + (3 * (ne + num_exposed)) + 2) = -1;

	  matstuff[ne + num_exposed] = 0;
	  num_exposed++;
	 }
      }
   }

 if (writeMesh(filename, framename, 
	       nn, pi, pj, totalele, 3, tri3, auxc, matstuff))
   {
    printf("problem writing mesh...continuing though\n");
   }

 delete matstuff;
 delete auxc;
 delete tri3;
 delete pj;
 delete pi;

 /* 
  *  OK, let's write the data sets
  */
 for (i = 0; i < nimp; i++)
   {
   }
}

 
int writeMesh(char *filename, char *framename,
	       int nnodes, float *pi, float *pj, float *nele, int nodesperele,
	       int *elemconn, int *auxconn, int *matstuff)
{
 /* 
  *  Open the file
  */
 VGeMeshFile *outfile = OpenVGeMeshFile(filename);
 
 if (outfile == NULL)
   {
    printf("Could not open File %s for output\n", filename);
    return (-1);
   }

 /* 
  *  Make sure we don't already have this frame
  */
 int numframe = NumFrames( outfile );
 int frameexists = 0;
 
 if (numframe > 0)
   {
    char *names = new char[(VGNAMELENMAX + 1) * numframe];
    numframe = QueryFrames( outfile, numframe, &names );

    int i;
    for (i = 0; i < numframe; i++)
      {
       if (!strcmp(names[i], framename))
	 {
	  frameexists = 1;
	  printf("Frame %s already exists in file %s\n", framename,
		 filename);
	  return (-1);
	 }
      }
   }
 
 /* 
  *  Create a frame and attach to it
  */
 int createframerc = CreateFrame( outfile, framename );
 
 if (createframerc == VMESH_FAIL)
   {
    printf("Couldn't create Frame %s in File %s\n", framename, filename);
    return (-1);
   }
 
 Frame *outframe = AttachFrame( outfile, framename, "w" );

 if (outframe == NULL)
   {
    printf("unexpected VMesh error\n");
    return (-1);
   }
 
 /* 
  *  Write out the mesh first
  */
 int createstructrc = CreateStruct( outframe, "Mesh" );
 
 if (createstructrc == VMESH_FAIL)
   {
    printf("Couldn't create Structure Vgroup\n");
    return (-1);
   }

 Struct *outstruct = AttachStruct( outframe, "Mesh", "w");
 
 if (outstruct == NULL)
   {
    printf("unexpected VMesh error\n");
    return (-1);
   }

 int putcoordrc = PutStructCoords2D( outstruct, nnodes, pi, pj );

 if (putcoordrc == VMESH_FAIL)
   {
    printf("unexpected VMesh error\n");
    return (-1);
   }

 int putconnectrc = PutStructElements(outstruct, "Tri3", nele, nodesperele,
				      elemconn);
 
 int putauxconnectrc = PutStructAuxiliary(outstruct, nele, nodesperele, 
					  auxconn);

 int putmat = PutMaterial(outstruct, nele, matstuff);
 
 if ((putconnectrc == VMESH_FAIL) ||
     (putauxconnectrc == VMESH_FAIL) ||
     (putmat == VMESH_FAIL))
   {
    printf("unexpected VMesh error\n");
    return (-1);
   }

 return (0);
}


 /* 
  *  OK, now write the data sets
  */
 for (isol = 0; isol < n_imp; isol++)
   {
    int createdatasetrc = CreateDataSet(outframe, 
					solname[soltoimp[isol]]);
    
    if (createdatasetrc == VMESH_FAIL)
      {
       printf("unexpected VMesh error\n");
       return(-1);
      }

    DataSet *newdata = AttachDataSet(outframe, solname[soltoimp[isol]], "w");
    
    if (newdata == NULL)
      {
       printf("unexpected VMesh error\n");
       return(-1);
      }

    int putdatarc = PutDataSet(newdata, outstruct, 
   }

 /* 
  *  Close the VGeMesh file and get the hell out of here
  */
 (void) CloseVGeMeshFile( outfile );

 return (0);
}

