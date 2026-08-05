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

/* The IDA fixed-width spellings (_BYTE/_WORD/_DWORD/_QWORD) that used to be typedef'd here were
 * removed 2026-08-05: every consumer now uses the canonical stdint spelling, and a decompiler
 * artifact shim is not Bungie-authentic (same call as the deleted hexrays_defs.h). */
