#ifndef __IMAGETOOL_H__
#define __IMAGETOOL_H__

#include "data_types.h"

int fill_grid(
int xsize,
int ysize,
float *xdata,
float *ydata,
float *data,
float *val,
int mat_num,
int win_xmin_index,
int win_xmax_index,
int win_ymin_index,
int win_ymax_index,
float min_value);

float interpolate(
float x,
float y,
int tri_num,
float *val);

int vertex_tri(
float x,
float y,
int tri_num);

int si_interface(
int pt_num);

int image_write(
char *filename,
char *par,
par_str* param);

int make_grid(
int xsize,
int ysize,
float **xd,
float **yd,
float win_xmin,
float win_xmax,
float win_ymin,
float win_ymax,
int *win_xmin_index,
int *win_xmax_index,
int *win_ymin_index,
int *win_ymax_index);

int shiftwindow(
float *array,
int size,
float mini,
float maxi,
int min_index,
int max_index);

float Fmax(
float x,
float y);

float Fmin(
float x,
float y);

int makeframe(
float *data,
int NX,
int NY,
int NXFAC,
int NYFAC,
char *name,
float min_value,
int mode,  /* scaling mode - for some extra "hooks" */
int macfile);

int mysmooth(
int value);

int window(
float x,
float y,
float *xarray,
float *yarray,
int xsize,
int ysize,
int *xlo,
int *xhi,
int *ylo,
int *yhi);

#endif
