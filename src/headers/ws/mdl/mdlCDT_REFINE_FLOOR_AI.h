#pragma once
#include "../cdt/cdtREFINE.h"
// ws-engine mdl — AI floor collision-refine query. DB-verified layout (types_members): a plain
// cdtREFINE (32 bytes) reached through two empty vtable-only derivations; the AI floor variant adds
// no data members, only overridden refine behaviour.

struct animINST; // ../anim/animINST.h — the querying instance (pInstSelf)  boundary (ptr)

// Empty (vtable-only) intermediate refine bases — no data members over cdtREFINE.
struct mdlCDT_REFINE_BASE    : cdtREFINE {};
struct mdlCDT_REFINE_BASE_AI : mdlCDT_REFINE_BASE {};

struct mdlCDT_REFINE_FLOOR_AI : mdlCDT_REFINE_BASE_AI {
    // ??0mdlCDT_REFINE_FLOOR_AI@@QAA@PAVanimINST@@@Z — construct for the querying instance
    // (null => a shared prototype filled in per query).  boundary.
    explicit mdlCDT_REFINE_FLOOR_AI(animINST *pInstSelf);
};
