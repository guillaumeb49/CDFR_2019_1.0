/*
 * F_Math.h
 *
 *  Created on: Nov 22, 2016
 *      Author: mario
 */

#ifndef F_MATH_H_
#define F_MATH_H_

#define COEF_180_PI	57.2957795	// 180 / PI
#define COEF_PI_180 0.01745329	// PI  / 180
#define PI			3.1415927
#define PI_X2 		6.2831853	// 2 * 3.14159..

#include <math.h>

float F_Math_toRad( float angle_deg);
float F_Math_toDeg( float angle_rad);
int   F_Math_Sign(float nb);
void  F_Math_ModuloPi(float *angleRad);
float F_Math_Abs(float nb);
float F_Math_Squared(float nb);

#endif /* F_MATH_H_ */
