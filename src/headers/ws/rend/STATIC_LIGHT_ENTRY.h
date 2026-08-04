#pragma once
#include "../lgt/lgtLIGHT.h"
// ws-engine rend: one entry of the per-update static-light candidate list (a light plus its
// evaluated attenuation). DB-verified layout (types_members STATIC_LIGHT_ENTRY): light@0(ptr),
// atten@4(float) — size 8.

typedef struct STATIC_LIGHT_ENTRY {
    lgtLIGHT *light; // 0x00
    float     atten; // 0x04 attenuation at the sample position
} STATIC_LIGHT_ENTRY;
