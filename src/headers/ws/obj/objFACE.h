#pragma once
#include <stdint.h>
// ws-engine obj: a single triangle face — three vertex indices into the object's vertex
// arrays. DB-verified layout (types_members objFACE) — size 6.

typedef struct objFACE {
    uint16_t ind[3]; // 0x00 vertex indices of the triangle corners
} objFACE;
