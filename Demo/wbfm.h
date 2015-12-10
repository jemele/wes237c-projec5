#ifndef WBFM_H
#define WBFM_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MYCOUNT  256 // it is MYCOUNT/2 complex numbers

void volk(float outputVector[MYCOUNT], float inputVector[MYCOUNT]);
void fir(float * result, float input, unsigned short do_filter);
void iir(float input, float *output, int size);
float fast_atan(float y, float x);
void xillybus_wrapper(float *in, float *out);

#endif
