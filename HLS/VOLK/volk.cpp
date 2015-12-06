/* VOLK */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "volk.h"

void volk(float outputVector[MYCOUNT], float inputVector[MYCOUNT])
{
#pragma HLS pipeline
	for(int i = 0; i <MYCOUNT-2; i+=2){
		const float r1 = inputVector[i];
		const float i1 = inputVector[i+1];
		const float r2 = inputVector[i+2];
		const float i2 = inputVector[i+3];

		outputVector[i] = (r1*r2)+(i1*i2);
		outputVector[i+1] = (r1*i2) - (i1*r2);
	}
}
