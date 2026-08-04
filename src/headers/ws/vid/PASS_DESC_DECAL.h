#pragma once
#include "PASS_DESC_DECAL_TEX.h"
// ws-engine vid: decal render-pass description. DB-verified layout (types_members
// PASS_DESC_DECAL) -- size 40. `tex` is the flattened PASS_DESC_DECAL::TEX bundle.

struct rendLIGHT_SET_SDR_COEFFS; // ../rend/rendLIGHT_SET_SDR_COEFFS.h -- boundary (pointer only)

typedef struct PASS_DESC_DECAL {
    PASS_DESC_DECAL_TEX tex;            // 0x00 (24)
    float               squaredDistToCam; // 0x18
    unsigned __int8     blendMode;      // 0x1C PASS_DESC_DECAL::BLENDMODE -- boundary enum
    float               glowIntensity;  // 0x20
    const rendLIGHT_SET_SDR_COEFFS *lightingCoeffs; // 0x24
} PASS_DESC_DECAL;                      // 40 bytes
