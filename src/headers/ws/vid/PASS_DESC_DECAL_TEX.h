#pragma once
#include "../txm/txmTEXTURE_PTR.h"
// ws-engine vid: the texture bundle for a decal render-pass description (diffuse/base/normal/
// detail-normal/height/cube maps). DB-verified layout (types_members PASS_DESC_DECAL::TEX):
// dif@0, base@4, nm@8, detnm@12, hm@16, cube@20 (each txmTEXTURE_PTR, 4B) -- size 24.
//
// NOTE: the real DB symbol is the nested class `PASS_DESC_DECAL::TEX`; the enclosing
// `PASS_DESC_DECAL` class is not decompiled elsewhere in this corpus. Flattened to
// `PASS_DESC_DECAL_TEX` per [[re-source-boundary-conventions]] scoping (mirrors the
// vidPASS_OBJ_MTL flattening).

typedef struct PASS_DESC_DECAL_TEX
{
    txmTEXTURE_PTR dif;   /* 0x00 */
    txmTEXTURE_PTR base;  /* 0x04 */
    txmTEXTURE_PTR nm;    /* 0x08 */
    txmTEXTURE_PTR detnm; /* 0x0C */
    txmTEXTURE_PTR hm;    /* 0x10 */
    txmTEXTURE_PTR cube;  /* 0x14 */

    // 0x82726D5C -- reversed in PASS_DESC_DECAL_TEX__TEX.c. Null-inits every slot.
    PASS_DESC_DECAL_TEX();
} PASS_DESC_DECAL_TEX;
