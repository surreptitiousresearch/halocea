#pragma once
#include "dmgTYPE.h"
// ws-engine dmg subsystem: the global damage system. Only the GetDmgType lookup is reached by this
// batch; the rest of the layout is the next frontier (kept opaque, handled by pointer).

struct dmgSYSTEM_vtbl;

struct dmgSYSTEM {
    dmgSYSTEM_vtbl *__vftable; // 0x00

    // 0x82B7C188 (?GetDmgType@dmgSYSTEM@@QBAPBVdmgTYPE@@E@Z) — const: look up the damage-type
    // descriptor for damage id `id`. Body external (dmg).  boundary.
    const dmgTYPE *GetDmgType(unsigned char id) const;
};

// ?dmgSystem@@3PAVdmgSYSTEM@@A — the process-wide damage system instance pointer.
extern dmgSYSTEM *dmgSystem;
