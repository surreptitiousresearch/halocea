#pragma once
#include "entSFX.h"
#include "../ds/dsTYPE_ID.h"
#include "../m3d/m3dCOLOR.h"
// ws-engine ent subsystem: color/intensity special-effect attachment. DB-verified layout
// (types_members entSFX_COLOR) — size 68 (0x44):
//   entSFX (base)@0 (36), forceAnimTime@0x24, curTime@0x28, firstCycle@0x2C (bool),
//   startColor@0x30 (m3dCOLOR, 16), startAKillValue@0x40, materialSwitched@0x41.
// The members end at 0x42; sizeof is 68 — m3dCOLOR's 4-byte alignment rounds the tail up, and
// .sweep/db_struct_sizes.tsv and .sweep/header_sizes.tsv both read 68 (the earlier "size 66"
// here was the member extent, not sizeof, contradicting the sibling entSFX/entSFX_LIGHT headers).

// Descriptor for entSFX_COLOR; only its runtime-type static is needed here.
struct entSFX_COLOR_DESC {
    static dsTYPE_ID TYPE_ID; // ?TYPE_ID@entSFX_COLOR_DESC@@2VdsTYPE_ID@@A @ 0x842CC5C0
};

typedef struct entSFX_COLOR : entSFX {
    float          forceAnimTime;   // 0x24
    float          curTime;         // 0x28
    bool           firstCycle;      // 0x2C
    m3dCOLOR       startColor;      // 0x30
    unsigned char  startAKillValue; // 0x40
    bool           materialSwitched;// 0x41

    // 0x82707F28 — force the colorizer's animation phase directly (writes forceAnimTime).
    void implSetIntensity(float intensity);
} entSFX_COLOR;
