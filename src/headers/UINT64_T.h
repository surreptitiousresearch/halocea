#pragma once
// Engine-wide 64-bit value split as two 32-bit halves (PowerPC has no native 64-bit GPR pair
// register class in this codebase's ABI surface for these fields; math is done by hand on
// hi/lo). DB-verified layout (types_members UINT64_T): hi@0, lo@4 — size 8.

typedef struct UINT64_T {
    unsigned int hi; // 0x00
    unsigned int lo; // 0x04
} UINT64_T;
