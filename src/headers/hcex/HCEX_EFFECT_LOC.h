#pragma once
#include "hcex_float3.h"
#include "../ws/ds/dsTSTRING.h"
// HCEX bridge: one placed instance of an effect — a position, a direction, and a name.
// DB-verified layout (types_members HCEX_EFFECT_LOC):
//   pos@0 (hcex_float3), vec@12 (hcex_float3), name@24 (dsTSTRING<char>) — size 28.

typedef struct HCEX_EFFECT_LOC {
    hcex_float3     pos;  // 0x00
    hcex_float3     vec;  // 0x0C
    dsTSTRING<char> name; // 0x18

    // 0x823D62B4 — default-construct: name adopts the shared empty-string singleton (pos/vec left
    // uninitialized, as observed in the disassembly).
    HCEX_EFFECT_LOC();
    // 0x823D4670 — copy-construct: copy pos/vec, share that.name's buffer (ref-bump).
    HCEX_EFFECT_LOC(const HCEX_EFFECT_LOC &that);
    // 0x823D1F78 — release name's buffer reference, freeing it at zero.
    ~HCEX_EFFECT_LOC();
    // 0x823D3E30 (`??_G` mangle) — scalar deleting destructor: run ~HCEX_EFFECT_LOC(), then
    // conditionally operator-delete `this` when bit 0 of `deleteFlags` is set.
    HCEX_EFFECT_LOC *ScalarDeletingDtor(unsigned char deleteFlags);
} HCEX_EFFECT_LOC;
