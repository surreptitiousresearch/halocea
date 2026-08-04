#pragma once
#include "cdtCOLL_ANIM_BASE_SCN.h"
// ws-engine cdt: per-scene collision-animation manager.
// DB-verified layout (types_members cdtCOLL_ANIM_SCN) — size 12 (0x0C).

struct animINST; // ../anim/animINST.h — instance being invalidated    boundary

struct cdtCOLL_ANIM_SCN_vtbl; // boundary — cdt subsystem vtable

typedef struct cdtCOLL_ANIM_SCN {
    cdtCOLL_ANIM_SCN_vtbl  *__vftable;      // 0x00
    cdtCOLL_ANIM_BASE_SCN  *pCollAnimSmall; // 0x04
    cdtCOLL_ANIM_BASE_SCN  *pCollAnimLarge; // 0x08

    // 0x82A25C70 — invalidate the cached collision-animation data for `pInst`. On first use
    // (idAnimCdt == 0xFF, a sentinel meaning "not yet classified") classifies the instance as
    // small/large by its OBB size (threshold 5.0) and caches that in idAnimCdt. `isDelete`!=0
    // invalidates BOTH the small and large scenes (instance teardown); otherwise only the scene
    // matching the cached classification is invalidated.
    void Invalidate(animINST *pInst, int isDelete);
} cdtCOLL_ANIM_SCN;
