#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ap_fixed.h>
#include "atan.h"

#if 1

typedef ap_fixed<64,3> fix_num;
const int NO_ITER = 16;
const fix_num pi2 = 1.57079632679;
fix_num angles[NO_ITER] = {0.785398163397448,	0.463647609000806,
    0.244978663126864,	0.124354994546761,	0.0624188099959574,
    0.0312398334302683,	0.0156237286204768,	0.00781234106010111,
    0.00390623013196697,	0.00195312251647882,	0.000976562189559320,
    0.000488281211194898,	0.000244140620149362,	0.000122070311893670,
    6.10351561742088e-05,	3.05175781155261e-05};

void correct_quad(float * x, float * y, int * quad)
  {

  	float temp = 0;

  	if (*x < 0) {
  		if (*y<0) {
  			temp = -*y; // temp = -1*y
  			* y = *x;
  			* x = temp;
  			*quad = 3;
  		} else {
  			temp = -*x;  // temp  = -1*x
  			* x = *y;
  			* y = temp;
  			*quad = 2;
  		}
  	}
  }

  void restore_quad(fix_num *theta, int quad){

  	if (quad == 2) {
  		*theta += (pi2);
  	}
  	if (quad == 3){
  		*theta -= (pi2);
  	}
  }


  float fast_atan(float x, float y)
  {
	// write your code here
	  	fix_num theta = 0;

	  	int quad = 0;
	  	correct_quad(&x, &y, &quad);
	  	fix_num temp = 0;

	  	fix_num xx = fix_num(x);
	  	fix_num yy = fix_num(y);

		for (int i = 0; i < NO_ITER; i++){
	  		if (yy <= 0) {
	  			temp=xx;
	  			xx -= yy>>i;
	  			yy += temp>>i;
	  			theta = theta - angles[i];
	  		} else {
	  			temp=xx;
	  			xx += yy>>i;
	  			yy -= temp>>i;
	  			theta = theta + angles[i];

	  		}
	  	}
	  	restore_quad(&theta, quad);
	  	return(float(theta));
  }

#else

  /***************************************************************************/

  /* Constant definitions */

  /***************************************************************************/

#define TAN_MAP_RES 0.003921549 /* (smallest non-zero value in table) */
#define RAD_PER_DEG 0.017453293
#define TAN_MAP_SIZE 255
//#define ZERO_TO_TWOPI
#define TWOPI 2*M_PI

  /* arctangents from 0 to pi/4 radians */

  static float

  fast_atan_table[257] = {
		  0.000000000, 0.003921549, 0.007842976, 0.011764160,
		  0.015684990, 0.019605330, 0.023525070, 0.027444090,
		  0.031362260, 0.035279470, 0.039195600, 0.043110530,
		  0.047024130, 0.050936290, 0.054846900, 0.058755820,
		  0.062662950, 0.066568160, 0.070471340, 0.074372380,
		  0.078271140, 0.082167520, 0.086061410, 0.089952670,
		  0.093841210, 0.097726910, 0.101609600, 0.105489300,
		  0.109365800, 0.113239000, 0.117108700, 0.120975000,
		  0.124837600, 0.128696500, 0.132551500, 0.136402600,
		  0.140249600, 0.144092400, 0.147931000, 0.151765200,
		  0.155594800, 0.159419900, 0.163240300, 0.167055900,
		  0.170866500, 0.174672200, 0.178472800, 0.182268100,
		  0.186058200, 0.189842800, 0.193622000, 0.197395600,
		  0.201163400, 0.204925500, 0.208681800, 0.212432000,
		  0.216176200, 0.219914300, 0.223646100, 0.227371600,
		  0.231090700, 0.234803300, 0.238509300, 0.242208600,
		  0.245901200, 0.249586900, 0.253265800, 0.256937600,
		  0.260602400, 0.264260000, 0.267910400, 0.271553500,
		  0.275189200, 0.278817500, 0.282438300, 0.286051400,
		  0.289656900, 0.293254700, 0.296844700, 0.300426800,
		  0.304000900, 0.307567100, 0.311125200, 0.314675200,
		  0.318217000, 0.321750600, 0.325275800, 0.328792700,
		  0.332301200, 0.335801200, 0.339292600, 0.342775500,
		  0.346249700, 0.349715300, 0.353172100, 0.356620100,
		  0.360059300, 0.363489600, 0.366911000, 0.370323400,
		  0.373726800, 0.377121100, 0.380506400, 0.383882500,
		  0.387249400, 0.390607000, 0.393955500, 0.397294600,
		  0.400624400, 0.403944800, 0.407255800, 0.410557400,
		  0.413849600, 0.417132200, 0.420405400, 0.423668900,
		  0.426922900, 0.430167300, 0.433402100, 0.436627200,
		  0.439842600, 0.443048300, 0.446244300, 0.449430600,
		  0.452607000, 0.455773800, 0.458930700, 0.462077800,
		  0.465215000, 0.468342400, 0.471460000, 0.474567600,
		  0.477665400, 0.480753200, 0.483831200, 0.486899200,
		  0.489957300, 0.493005500, 0.496043700, 0.499071900,
		  0.502090200, 0.505098500, 0.508096800, 0.511085200,
		  0.514063600, 0.517032000, 0.519990400, 0.522938800,
		  0.525877200, 0.528805600, 0.531724100, 0.534632500,
		  0.537531000, 0.540419500, 0.543298000, 0.546166600,
		  0.549025100, 0.551873800, 0.554712400, 0.557541100,
		  0.560359900, 0.563168700, 0.565967600, 0.568756600,
		  0.571535700, 0.574304800, 0.577064100, 0.579813500,
		  0.582553100, 0.585282800, 0.588002600, 0.590712600,
		  0.593412800, 0.596103200, 0.598783900, 0.601454700,
		  0.604115800, 0.606767200, 0.609408800, 0.612040700,
		  0.614663000, 0.617275500, 0.619878400, 0.622471700,
		  0.625055400, 0.627629400, 0.630193900, 0.632748800,
		  0.635294200, 0.637830100, 0.640356500, 0.642873400,
		  0.645380800, 0.647878800, 0.650367400, 0.652846600,
		  0.655316500, 0.657777000, 0.660228200, 0.662670100,
		  0.665102700, 0.667526100, 0.669940200, 0.672345200,
		  0.674740900, 0.677127600, 0.679505100, 0.681873500,
		  0.684232800, 0.686583100, 0.688924400, 0.691256700,
		  0.693580000, 0.695894300, 0.698199800, 0.700496400,
		  0.702784100, 0.705063000, 0.707333000, 0.709594300,
		  0.711846900, 0.714090700, 0.716325800, 0.718552300,
		  0.720770100, 0.722979400, 0.725180000, 0.727372100,
		  0.729555700, 0.731730700, 0.733897400, 0.736055500,
		  0.738205300, 0.740346700, 0.742479700, 0.744604500,
		  0.746720900, 0.748829100, 0.750929100, 0.753020800,
		  0.755104400, 0.757179800, 0.759247200, 0.761306400,
		  0.763357600, 0.765400800, 0.767436000, 0.769463300,
		  0.771482600, 0.773494000, 0.775497500, 0.777493200,
		  0.779481100, 0.781461200, 0.783433500, 0.785398200,
		  0.785398200
  };

  /*****************************************************************************

   Function: Arc tangent


   Syntax: angle = fast_atan2(y, x);

   float y y component of input vector

   float x x component of input vector

   float angle angle of vector (x, y) in radians


   Description: This function calculates the angle of the vector (x,y)

   based on a table lookup and linear interpolation. The table uses a

   256 point table covering -45 to +45 degrees and uses symetry to

   determine the final angle value in the range of -180 to 180

   degrees. Note that this function uses the small angle approximation

   for values close to zero. This routine calculates the arc tangent

   with an average error of +/- 3.56e-5 degrees (6.21e-7 radians).

  *****************************************************************************/

  float fast_atan(float x, float y)
  {
#pragma HLS pipeline

	float x_abs, y_abs, z;
	float alpha, angle, base_angle;
	int index;

	/* normalize to +/- 45 degree range */
	y_abs = fabsf(y);
	x_abs = fabsf(x);
	/* don't divide by zero! */
	if(!((y_abs > 0.0f) || (x_abs > 0.0f)))
		return 0.0;

	if(y_abs < x_abs)
		z = y_abs / x_abs;
	else
		z = x_abs / y_abs;

	/* when ratio approaches the table resolution, the angle is */
	/* best approximated with the argument itself... */
	if(z < TAN_MAP_RES)
		base_angle = z;
	else {
		/* find index and interpolation value */
		alpha = z * (float)TAN_MAP_SIZE;
		index = ((int)alpha) & 0xff;
		alpha -= (float)index;
		/* determine base angle based on quadrant and */
		/* add or subtract table value from base angle based on quadrant */
		base_angle  =  fast_atan_table[index];
		base_angle += (fast_atan_table[index + 1] - fast_atan_table[index]) * alpha;
	}

	if(x_abs > y_abs) { /* -45 -> 45 or 135 -> 225 */
		if(x >= 0.0) { /* -45 -> 45 */
		  if(y >= 0.0)
			angle = base_angle; /* 0 -> 45, angle OK */
		  else
			angle = -base_angle; /* -45 -> 0, angle = -angle */
		}
		else { /* 135 -> 180 or 180 -> -135 */
		  angle = 3.14159265358979323846;
		  if(y >= 0.0)
			angle -= base_angle; /* 135 -> 180, angle = 180 - angle */
		  else
			angle = base_angle - angle; /* 180 -> -135, angle = angle - 180 */
		}
	}
	else { /* 45 -> 135 or -135 -> -45 */
		if(y >= 0.0) { /* 45 -> 135 */
		  angle = 1.57079632679489661923;
		  if(x >= 0.0)
			angle -= base_angle; /* 45 -> 90, angle = 90 - angle */
		  else
			angle += base_angle; /* 90 -> 135, angle = 90 + angle */
		}
		else { /* -135 -> -45 */
		  angle = -1.57079632679489661923;
		  if(x >= 0.0)
			angle += base_angle; /* -90 -> -45, angle = -90 + angle */
		  else
			angle -= base_angle; /* -135 -> -90, angle = -90 - angle */
		}
	}


	return (angle);

  }
#endif
