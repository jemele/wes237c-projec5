#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ap_fixed.h>
#include "atan.h"

  /***************************************************************************/

  /* Constant definitions */

  /***************************************************************************/

typedef ap_fixed<64,3> fix_num;

  /* arctangents from 0 to pi/4 radians */
const int NO_ITER = 16;
const fix_num pi2 = 1.57079632679;
fix_num angles[NO_ITER] = {0.785398163397448,	0.463647609000806,	0.244978663126864,	0.124354994546761,	0.0624188099959574,	0.0312398334302683,	0.0156237286204768,	0.00781234106010111,	0.00390623013196697,	0.00195312251647882,	0.000976562189559320,	0.000488281211194898,	0.000244140620149362,	0.000122070311893670,	6.10351561742088e-05,	3.05175781155261e-05};


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

