#pragma once
#include "bhpPARAMS.h"
// ws-engine bhp subsystem — "stay" behaviour-parameter block. DB-verified layout
// (types_members bhpSTAY_PAR) — size 44 (0x2C): bhpPARAMS base@0 (40B), forceKeepPos@0x28 (bool).

struct bhpSTAY_PAR : bhpPARAMS {
    bool forceKeepPos; // 0x28

    // Default-construct an empty stay param block. Body external (bhp).  boundary.
    bhpSTAY_PAR();
    // Destroy (restore bhpPARAMS vtable, free the base fail-notify vector; inlined at call sites).
    // Body external (bhp).  boundary.
    ~bhpSTAY_PAR();
};
