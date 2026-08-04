#pragma once
#include "HCEX_EFFECT_LOC.h"
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/dsTSTRING.h"
// HCEX bridge: a named effect and its set of placed locations.
// DB-verified layout (types_members HCEX_EFFECT):
//   locations@0 (dsVECTOR<HCEX_EFFECT_LOC,8>), hcexName@20 (dsTSTRING<char>) — size 24.

typedef struct HCEX_EFFECT {
    dsVECTOR<HCEX_EFFECT_LOC, 8> locations; // 0x00
    dsTSTRING<char>              hcexName;  // 0x14

    // 0x823D6AA4 — default-construct: adopts the ds_vector.h:24 call-site cookie for `locations`
    // (empty vector), hcexName adopts the shared empty-string singleton.
    HCEX_EFFECT();
    // 0x823E4350 — copy-construct: adopt that.locations's call-site cookie, deep-copy its elements
    // via Insert, then share that.hcexName's buffer (ref-bump).
    HCEX_EFFECT(const HCEX_EFFECT &that);
    // 0x823D6B54 — release hcexName's buffer reference, then destroy the locations vector.
    ~HCEX_EFFECT();
    // 0x823D7C10 (`??_G` mangle) — scalar deleting destructor: run ~HCEX_EFFECT(), then
    // conditionally operator-delete `this` when bit 0 of `deleteFlags` is set.
    HCEX_EFFECT *ScalarDeletingDtor(unsigned char deleteFlags);
} HCEX_EFFECT;
