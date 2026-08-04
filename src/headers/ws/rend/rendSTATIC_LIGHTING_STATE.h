#pragma once
#include "shCOLOR.h"
#include "rendLIGHT_SET_SDR_COEFFS.h"
// ws-engine rend: baked static-lighting state carried by a rendLIGHT_SET (desired vs. current).
// DB-verified layout (types_members rendSTATIC_LIGHTING_STATE): shColor@0(108),
// sdrCoeffs@108(128) — size 236.

typedef struct rendSTATIC_LIGHTING_STATE {
    shCOLOR                   shColor;   // 0x00
    rendLIGHT_SET_SDR_COEFFS  sdrCoeffs; // 0x6C
} rendSTATIC_LIGHTING_STATE;
