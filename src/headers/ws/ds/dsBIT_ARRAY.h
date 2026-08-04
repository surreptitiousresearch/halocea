#pragma once
#include "../../apCL.h"
// ws-engine ds: heap-backed bit array (wraps the dsBIT_ARRAY_IMPL word helpers).
// DB-verified layout (types_members dsBIT_ARRAY, size 16 / 0x10):
//   nElem@0 (int, 32-bit word count), array@4 (unsigned int*), __cl@8 (apCL, 8).

typedef struct dsBIT_ARRAY {
    int           nElem; // 0x00 word (32-bit cell) count
    unsigned int *array; // 0x04 bit storage
    apCL          __cl;  // 0x08 allocation call-site cookie
} dsBIT_ARRAY;
