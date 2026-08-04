#pragma once
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dV.h"
// ws-engine ai08: base class for a brain's aim-behaviour module.
// DB-verified layout (types_members aiAIM_BASE): __vftable@0 — size 4.
// The vtable (??_7aiAIM_BASE@@6B@) is modeled as the __vftable data member; the methods below are
// its slots, in slot order. Calls are modeled as regular member calls (the codebase convention: the
// vtable is data, not a C++ `virtual` table), matching aiENEMY_BASE / aiSND_BASE.
//   slot 0 (+0x00): scalar-deleting destructor
//   slot 1 (+0x04): ProcessFrame
//   slot 2 (+0x08): ReStart
//   slot 3 (+0x0C): SetTrk
//   slot 4 (+0x10): DropTrk
//   slot 5 (+0x14): GetGivenAim (const)
//   slot 6 (+0x18): IsGiven     (const)
//   slot 7 (+0x1C): GetPos      (const)

struct aiAIM_BASE_vtbl;
struct entTRACKER;

struct aiAIM_BASE {
    aiAIM_BASE_vtbl *__vftable; // 0x00

    // 0x83174848 — per-frame aim update.  boundary.
    void ProcessFrame();
    // 0x83174850 — restart the aim behaviour (reset internal aiming state).  boundary.
    void ReStart();
    // 0x83174858 (?SetTrk@aiAIM_BASE@@UAAXPAVentTRACKER@@@Z) — install the aim tracker.  boundary.
    void SetTrk(entTRACKER *pTrk);
    // 0x83174860 (?DropTrk@aiAIM_BASE@@UAAXXZ) — drop the aim tracker.  boundary.
    void DropTrk();
    // 0x83179290 (?GetGivenAim@aiAIM_BASE@@UBA?AV?$dsTSTRING@D@@XZ) — const.
    // Base default: return the empty string (sret). REVERSED: aiAIM_BASE__GetGivenAim.cpp.
    dsTSTRING<char> GetGivenAim() const;
    // 0x83174868 (?IsGiven@aiAIM_BASE@@UBAHXZ) — const: is an aim currently assigned.  boundary.
    int IsGiven() const;
    // 0x83174870 (?GetPos@aiAIM_BASE@@UBA?AUm3dV@@XZ) — const: current aim position (sret).  boundary.
    m3dV GetPos() const;
};
