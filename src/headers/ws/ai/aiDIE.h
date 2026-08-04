#pragma once
// ws-engine ai08 — the brain's death/dying module.
// DB-verified layout (types_members aiDIE): aiDIE_BASE base@0, pBrain@4 (aiBRAIN*) — size 8.
#include "aiDIE_BASE.h"

struct aiBRAIN;    // aiBRAIN.h
struct dmgDAMAGE;  // dmg/dmgDAMAGE.h

struct aiDIE : aiDIE_BASE {
    // 0x832AA... (??0aiDIE@@QAA@PAVaiBRAIN@@@Z) — construct the death/dying module for the brain.
    aiDIE(aiBRAIN *brain);

    aiBRAIN *pBrain; // 0x04 owning brain

    // 0x832AA7B0 (?DropWeaponAndItems@aiDIE@@UAAXXZ) — virtual: if the drop-on-death debug gate is
    // set and the brain has not yet dropped, log and throw all held weapons/items.
    // REVERSED: src/ws/ai/aiDIE__DropWeaponAndItems.cpp.
    void DropWeaponAndItems();

    // 0x832AA828 (?Start@aiDIE@@UAAXPAVdmgDAMAGE@@@Z) — virtual: begin the death sequence for the
    // brain (force delayed calls, mark dead, unregister from planner, notify subscribers, run the
    // dead behaviour, record killer/type). REVERSED: src/ws/ai/aiDIE__Start.cpp.
    void Start(dmgDAMAGE *dmg);
};
