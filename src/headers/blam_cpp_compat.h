#pragma once
/* blam_cpp_compat.h — force-included (cl /FI) into every C++ TU by the C++ build/check
 * scripts (clcheck_cpp.bat). The Hex-Rays decompiler emits IDA/GCC spellings that MSVC C++
 * does not provide as builtins. bool/true/false/nullptr ARE C++ keywords, so unlike the C
 * shim we only supply the IDA fixed-width integer aliases, BOOL, and __noreturn. No layout
 * or codegen impact. */

/* Blam BOOL is a 4-byte signed int (matches Windows BOOL). */
#ifndef BLAM_BOOL_DEFINED
#define BLAM_BOOL_DEFINED
typedef int BOOL;
#endif

/* Decompiler emits GCC-style `__noreturn`; map to the MSVC attribute spelling. */
#ifndef __noreturn
#define __noreturn __declspec(noreturn)
#endif

/* IDA fixed-width integer spellings emitted by the decompiler. */
#ifndef BLAM_IDA_INTS_DEFINED
#define BLAM_IDA_INTS_DEFINED
typedef unsigned char      _BYTE;
typedef unsigned __int16   _WORD;
typedef unsigned __int32   _DWORD;
typedef unsigned __int64   _QWORD;
#endif

/* PPC floating-point intrinsics the Hex-Rays PPC decompiler emits inline. On the Xbox 360
 * target these are single-instruction ops; modeled here as their exact scalar equivalents so
 * reconstructed TUs compile on the x64 host gate. (fsqrts = single-precision sqrt; fabs = |x|.) */
#ifndef BLAM_PPC_FP_INTRINSICS
#define BLAM_PPC_FP_INTRINSICS
static inline double __fabs(double x)  { return x < 0.0 ? -x : x; }
extern "C" float sqrtf(float);  /* C linkage (matches <math.h>); a block-scope extern is C++-linkage and clashes 2026-07-31 */
static inline float  __fsqrts(float x) { return sqrtf(x); }
#endif
