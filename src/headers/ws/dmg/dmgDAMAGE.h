#pragma once
#include "../msg/msgDATA.h"
#include "../ds/WEAK_PTR_BASE.h"
#include "../ds/dsTSTRING.h"
#include "dspPARAMS.h"
// ws-engine dmg subsystem: a single damage event delivered to an entity.
// DB-verified layout (types_members dmgDAMAGE): msgDATA@0 (4B, base), ds::WEAK_PTR_BASE<dmgDAMAGE>@4,
// params@8 (dspPARAMS, 120B), pDmgType@128, amount@132, stun@136, impulse@140, isRemote@144 —
// total size 148 (0x94).

struct dmgTYPE;

struct dmgDAMAGE : msgDATA {
    ds::WEAK_PTR_BASE<dmgDAMAGE> weakSelf;  // 0x04 weak-referenceable identity
    dspPARAMS                    params;    // 0x08 shooter/sender/geometry
    const dmgTYPE               *pDmgType;  // 0x80 damage-type descriptor
    float                        amount;    // 0x84 damage amount
    float                        stun;      // 0x88
    float                        impulse;   // 0x8C
    int                          isRemote;  // 0x90

    // 0x82B79C58 (?GetType@dmgDAMAGE@@QBA?AV?$dsTSTRING@D@@XZ) — the damage type's name as an
    // interned string (returned by value). boundary.
    dsTSTRING<char> GetType() const;

    // Construct a damage event of type `pDmgType` with the given `amount` (stun/impulse defaulted).
    // Body external (dmg).  boundary.
    dmgDAMAGE(const dmgTYPE *pDmgType, float amount);
    // Destroy: tear down params and release the weak-referenceable identity handle (inlined at call
    // sites). Body external (dmg).  boundary.
    ~dmgDAMAGE();
};
