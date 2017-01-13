/********************************************************************
 * Handles floating point craps for Suprem4.gs resurrection project.
 *
 * Written by Taylor Shin
 * Argonne National Laboratory
 * Apr. 26th, 2016
 *
 ********************************************************************/
#include <math.h>
#include <limits.h>
#include <float.h>

#include "local_math.h"

// Floating point comparator with epsilon comparison method

// A == B?
int f_equal(float inputA, float inputB)
{
	if ( fabsf(inputA-inputB) > FLT_EPSILON )
		return 0;
	else
		return 1;
}
// A < B?
int f_lt(float inputA, float inputB)
{
	if (f_equal(inputA, inputB))
		return 0;
	else {
		if (inputA < inputB)
			return 1;
		else
			return 0;
	}
}
// A > B?
int f_mt(float inputA, float inputB)
{
	if (f_equal(inputA, inputB))
		return 0;
	else {
		if (inputA > inputB)
			return 1;
		else
			return 0;
	}
}

// Double precision comparator with epsilon comparison method
// A == B?
int d_equal(double inputA, double inputB)
{
	if ( fabs(inputA-inputB) > DBL_EPSILON )
		return 0;
	else
		return 1;
}
// A < B?
int d_lt(double inputA, double inputB)
{
	if (d_equal(inputA, inputB))
		return 0;
	else {
		if (inputA < inputB) return 1;
		else return 0;
	}
}
// A > B?
int d_mt(double inputA, double inputB)
{
	if (d_equal(inputA, inputB))
		return 0;
	else {
		if (inputA > inputB) return 1;
		else return 0;
	}
}
