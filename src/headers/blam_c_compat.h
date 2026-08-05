#pragma once
/* blam_c_compat.h — force-included (cl /FI) into every C TU by the build/check scripts.
 * The decompiler emits C++/C23 spellings (`bool`, `true`, `false`, `nullptr`) that this MSVC
 * version's C mode does not provide as keywords; map them to their exact C equivalents.
 * No layout or codegen impact: bool is the 1-byte _Bool, nullptr is ((void*)0). */

#ifndef __cplusplus
#include <stdint.h>
#include <stdbool.h>   /* bool / true / false */
#ifndef nullptr
#define nullptr ((void *)0)
#endif
#endif

/* Blam BOOL is a 4-byte signed int (matches Windows BOOL). Plain `BOOL` is used as a
 * return/param type across the reconstructed Blam TUs but is not otherwise declared. */
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
typedef uint16_t   _WORD;
typedef uint32_t   _DWORD;
typedef uint64_t   _QWORD;
#endif
