#pragma once
#include "bhpPARAMS.h"
// ws-engine bhp subsystem — "dead" behaviour-parameter block. DB-verified layout
// (types_members bhpDEAD_PAR) — size 44 (0x2C): bhpPARAMS base@0 (40B), dmg@0x28 (dmgDAMAGE*).

struct dmgDAMAGE; // dmg — the damage event that killed the unit (fwd)

struct bhpDEAD_PAR : bhpPARAMS {
    dmgDAMAGE *dmg; // 0x28

    // Default-construct an empty dead param block. Body external (bhp).  boundary.
    bhpDEAD_PAR();
    // Destroy (restore bhpPARAMS vtable, free the base fail-notify vector; inlined at call sites).
    // Body external (bhp).  boundary.
    ~bhpDEAD_PAR();
};
