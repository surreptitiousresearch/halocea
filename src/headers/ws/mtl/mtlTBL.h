#pragma once
#include "../ds/dsVECTOR.h"
#include "mtlMTL.h"
#include "mtlTEX_DENSITY.h"
// ws-engine mtl: material lookup table. DB-verified layout (types_members mtlTBL):
//   mtlList@0 (dsVECTOR<mtlMTL,8>, 20B), texDensity@20 (mtlTEX_DENSITY, 40B) — size 60.

typedef struct mtlTBL {
    dsVECTOR<mtlMTL, 8> mtlList;    // 0x00 — resolved material slots
    mtlTEX_DENSITY      texDensity; // 0x14 — per-slot texel-density scale (u[]/v[])

    // 0x82C2ABD8 — reset texDensity.u[]/v[] to 1.0 (default, unscaled density).
    void Init();

    // 0x82C2CCF8 — clear mtlList, reclaim its storage, and restore default state via Init().
    void Clear();

    // 0x82C2B3A8 (const) — linear scan of mtlList for the slot whose `mtl` key matches;
    // direction selects scan order (0 = forward from startPos, 1 = backward from
    // nElem-startPos-1, any other value = not-found). Returns the slot index, or -1.
    int _FindMtl(int mtl, int startPos, int direction) const;

    // 0x82C2CC98 — _FindMtl(mtl, 0, 0) resolved to a pointer into mtlList, or nullptr
    // if no match. startPos/direction are accepted but not forwarded (see .cpp caveat).
    mtlMTL *FindMtl(int mtl, int startPos, int direction);

    // Deep-copy construct (used when duplicating an objSPLIT_ENTRY). Body not decompiled here.
    // Accreted from the objSPLIT_ENTRY::mtlTbl usage site — boundary.
    mtlTBL(const mtlTBL &src); // boundary

    // 0x828835E4 — default-construct: empty mtlList (adopting the material.h:210 allocation
    // call-site cookie), default texDensity.
    mtlTBL();
} mtlTBL;
