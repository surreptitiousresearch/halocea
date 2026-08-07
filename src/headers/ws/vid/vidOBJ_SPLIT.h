#pragma once
#include "vidOBJ_SPLIT_OCC_RES.h"
#include "../txm/txmTEXTURE_PTR.h"
#include "../../d3dOBJ_PASS_LIST.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "vidMTL.h"
#include "vidPASS_OBJ.h"
// ws-engine vid: one draw-batch split of a render object, grouped by material/pass state.
// DB-verified layout (types_members vidOBJ_SPLIT): occRes@0 (vidOBJ_SPLIT_OCC_RES, 16B),
// passList@16 (d3dOBJ_PASS_LIST, 88B), objDesc@104 (vidPASS_OBJ, 156B), mtlList@260
// (dsVECTOR<vidMTL,8>, 20B), curMtl@280 (char) — size 281 (rounds up with tail padding).
//
// CAVEAT: passList (d3dOBJ_PASS_LIST) is a large, deeply-nested D3D pass-description type
// outside this batch's closure; per the established vid* boundary convention (large
// self-contained video-driver classes reached through a handful of methods) it is kept as a
// DB-size-accurate opaque byte blob. objDesc (vidPASS_OBJ, DB-verified types_members
// vidPASS_OBJ) IS typed, because dsVECTOR<vidOBJ_SPLIT,8>::ShrinkResize/Resize destroy the
// material sub-object at objDesc.mtl (offset 24 within vidPASS_OBJ) directly.
//
// vidPASS_OBJ used to have a SECOND body right here — same nine DB members and the same 156
// bytes, but with explicit _pad members and with `mtl` typed as a locally-nested `struct MTL`
// instead of vidPASS_OBJ_MTL. Two file-scope bodies are `error: redefinition` in the
// header_layout probe TU and made the layout a TU saw depend on include order, so the canonical
// (ws/vid/vidPASS_OBJ.h) is now included instead; the nested enums this copy carried were merged
// into it. Consequence for this header's own consumers: the material sub-object is spelled
// vidPASS_OBJ_MTL (the corpus's single model of the DB type vidPASS_OBJ::MTL — its per-member
// ctors are reversed in src/vidPASS_OBJ_MTL*.c), so the two dsVECTOR TUs call
// `objDesc.mtl.~vidPASS_OBJ_MTL()` rather than `.~MTL()`.

typedef struct vidOBJ_SPLIT {
    vidOBJ_SPLIT_OCC_RES  occRes;          // 0x000
    d3dOBJ_PASS_LIST      passList;        // 0x010 (see ../../d3dOBJ_PASS_LIST.h)
    vidPASS_OBJ           objDesc;         // 0x068
    dsVECTOR<vidMTL, 8>   mtlList;         // 0x104 resolved material slots for this split
    char                  curMtl;          // 0x118 index (as a signed byte) of the currently-bound material

    // 0x82C87370 (const) -- linear scan of mtlList for the slot whose name matches `name`;
    // returns its index, or -1 if not found.
    int FindMtl(const dsTSTRING<char> &name) const;

    // Default-construct: boundary (only its call sites are reversed).
    vidOBJ_SPLIT(); // boundary
} vidOBJ_SPLIT;
