#pragma once
#include "shSHD_COEFFS.h"
#include "../m3d/m3dCOLOR.h"
// ws-engine rend: shader-directional-response coefficients for a static light state.
// DB-verified layout (types_members rendLIGHT_SET_SDR_COEFFS): shCoeffs@0(112), ambient@112(16)
// — size 128.

typedef struct rendLIGHT_SET_SDR_COEFFS {
    shSHD_COEFFS shCoeffs; // 0x00
    m3dCOLOR     ambient;  // 0x70
} rendLIGHT_SET_SDR_COEFFS;
