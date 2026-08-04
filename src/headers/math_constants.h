#ifndef MATH_CONSTANTS_H
#define MATH_CONSTANTS_H

/* Degree/radian conversion constants as compiled into the binary (single-precision
 * rounding of pi/180 and 180/pi; e.g. __real_3c8efa35 = 0.017453292f). Named here so
 * call sites read as unit conversions instead of magic floats. */
#define DEG_TO_RAD 0.017453292f
#define RAD_TO_DEG 57.29578f

/* pi and 2*pi, single-precision. */
#define PI     3.1415927f
#define TWO_PI 6.2831855f

/* 1/(2*pi), single-precision (radians -> [0,1] turn fraction). */
#define INV_TWO_PI 0.15915494f

#endif /* MATH_CONSTANTS_H */
