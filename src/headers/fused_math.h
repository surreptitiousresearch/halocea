#pragma once
/* fused_math — portable spellings of the PPC single-precision fused multiply-add family.
 * PPC fmadds/fmsubs/fnmsubs compute a*b±c with ONE rounding; a plain C `a*b + c` rounds
 * twice and drifts 1-2 ULP from the retail binary (verified by external tick-parity
 * comparison, bug_reports/ 2026-08-16). C99 fmaf() is the exact portable equivalent on any
 * IEEE target (user decision 2026-08-17: fused sites are respelled through these helpers).
 * Use ONLY where the disassembly shows the fused instruction; plain expressions stay plain. */
#include <math.h>

/* fmadds f, a, b, c  ->  a*b + c, single rounding */
static inline float fused_madd(float a, float b, float c) { return fmaf(a, b, c); }

/* fmsubs f, a, b, c  ->  a*b - c, single rounding */
static inline float fused_msub(float a, float b, float c) { return fmaf(a, b, -c); }

/* fnmsubs f, a, b, c  ->  -(a*b - c) = c - a*b, single rounding */
static inline float fused_nmsub(float a, float b, float c) { return fmaf(-a, b, c); }
