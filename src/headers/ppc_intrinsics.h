#pragma once
/* ppc_intrinsics — PowerPC floating-point compiler intrinsics surfaced by the decompiler. __fsqrts is the
 * single-precision hardware square root (fsqrts); __fabs is the floating absolute value (fabs). Declared as
 * boundary builtins so the reconstructed math reads as written. */

extern float  __fsqrts(float x);
extern double __fabs(double x);
