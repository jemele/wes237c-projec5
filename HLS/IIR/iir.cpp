//IIR Filter with predetermined taps

#include <stdio.h>

static float a1 = 0.928849;
static float b0 = 0.964424;
static float b1 = 0.964424;

static float input_minus_one = 0.0;
static float output_minus_one = 0.0;

void iir(float input, float *output, int size)
{
	//write your code here
	
	*output = input*b0+input_minus_one*b1 + output_minus_one*a1;
	output_minus_one = *output;
	input_minus_one = input;
}
