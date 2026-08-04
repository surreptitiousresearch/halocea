#pragma once
#include "entSFX.h"
#include "../ds/dsTYPE_ID.h"
#include "../m3d/m3dCOLOR.h"
// ws-engine ent subsystem: color/intensity special-effect attachment. DB-verified layout
// (types_members entSFX_COLOR) — size 66 (0x42):
//   entSFX (base)@0 (36), forceAnimTime@0x24, curTime@0x28, firstCycle@0x2C (bool),
//   startColor@0x30 (m3dCOLOR, 16), startAKillValue@0x40, materialSwitched@0x41.

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
