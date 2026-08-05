#pragma once
#include <stdint.h>
#include "vidPASS_OBJ_MTL.h"
// ws-engine vid: per-object render-pass state. DB-verified layout (types_members vidPASS_OBJ)
// -- size 156. Only the `mtl` sub-object (the flattened vidPASS_OBJ::MTL) is fully modelled here;
// the small scalar/enum fields ahead of it are named at their DB offsets.

typedef struct vidPASS_OBJ {
    uint8_t  coordSpace; // 0x00 vidPASS_OBJ::COORDSPACE -- boundary enum
    uint8_t  skinType;   // 0x01 vidPASS_OBJ::SKINTYPE   -- boundary enum
    uint8_t  billboard;  // 0x02 vidPASS_OBJ::BILLBOARD  -- boundary enum
    float            z_bias;     // 0x04
    float            z_bias_ss;  // 0x08
    float            scale;      // 0x0C
    bool             decal;      // 0x10
    unsigned int     geom;       // 0x14
    vidPASS_OBJ_MTL  mtl;        // 0x18 (132) flattened vidPASS_OBJ::MTL
} vidPASS_OBJ;                    // 156 bytes
