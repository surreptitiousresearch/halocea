#pragma once
// ws-engine anim: base type for animation commands dispatched through animMNG::SendAnimCmd.
// DB-verified layout (types_members animCMD_BASE): __vftable@0, layerID@4 — size 8.

struct animCMD_BASE;

// DB: animCMD_BASE_vtbl — command dispatch table (slots external to this batch).
struct animCMD_BASE_vtbl;

struct animMNG; // animMNG.h — dispatching manager (pointer only)  boundary

typedef struct animCMD_BASE {
    animCMD_BASE_vtbl *__vftable; // 0x00
    int                layerID;   // 0x04 target animation layer

    // Resolve which layer of `mng` this command targets (vtbl slot): a concrete layer index,
    // -1 (none), or -2 (broadcast to every layer). boundary (virtual).
    int GetDestLayer(animMNG *mng) const; // boundary (virtual)
} animCMD_BASE;
