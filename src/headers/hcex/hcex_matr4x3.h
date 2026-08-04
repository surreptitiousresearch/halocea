#pragma once
/* hcex_matr4x3 — HCEX's row-major 4x3 transform (52 bytes), layout-identical to Blam's real_matrix4x3:
 * a uniform scale followed by the forward/left/up basis vectors and a position. Used to shuttle object
 * transforms across the Blam<->ws bridge. */

#include "hcex_float3.h"   /* hcex_float3 — this is the only dependency hcex_matr4x3 actually has.
                            * DEVIATION: previously included the much heavier
                            * hcex_prepare_locations_boundary.h (for the same hcex_float3), which also
                            * defines its own plain-C m3dV/m3dMATR/m4dV/objOBJ stand-ins. Those collide
                            * (duplicate struct/union definitions) with the full ws:: math/obj headers
                            * in any translation unit that needs both this type and the real
                            * ws::animINST/ws::m3dMATR (e.g. hcex_update_fp_model). Consumers that
                            * relied on the transitive m3dV/m3dMATR/objOBJ pull now include
                            * hcex_prepare_locations_boundary.h explicitly. */

typedef struct hcex_matr4x3
{
    float       scale;     /* 0x00 */
    hcex_float3 forward;   /* 0x04 */
    hcex_float3 left;      /* 0x10 */
    hcex_float3 up;        /* 0x1C */
    hcex_float3 pos;       /* 0x28 */
} hcex_matr4x3;
