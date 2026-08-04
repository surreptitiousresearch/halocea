#pragma once
#include "anim_blend_SEQ_INFO.h"
// ws-engine anim: abstract base for a blend layer held in animMNG::layerList.
// DB-verified layout (types_members animLAYER_BASE): __vftable@0 — size 4. Only the vtable
// slots the anim manager dispatches through are modeled; the concrete layer state is opaque.

struct animCMD_BASE;      // animCMD_BASE.h — animation command (pointer only)   boundary
struct animUPDATE_STATUS; // animUPDATE_STATUS.h — command out-status (pointer)  boundary
struct animLAYER_BASE;

// DB: animLAYER_BASE_vtbl — layer dispatch table (only the slots used here are named).
struct animLAYER_BASE_vtbl;

typedef struct animLAYER_BASE {
    animLAYER_BASE_vtbl *__vftable; // 0x00

    // Return the layer's currently-playing sequence record (nullptr if none). boundary (virtual).
    anim_blend::SEQ_INFO *GetCurAnimSeq(); // boundary (virtual)

    // Apply an animation command to this layer, reporting any pose change via `status`.
    void HandleCmd(const animCMD_BASE *cmd, animUPDATE_STATUS *status); // boundary (virtual)
} animLAYER_BASE;
