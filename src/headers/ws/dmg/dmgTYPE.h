#pragma once
#include "../ds/dsSTRID.h"
#include "../dsc/dscDESC.h"
#include "../ap/apSTATE_T.h"
// ws-engine dmg subsystem: a damage-type descriptor. DB-verified partial layout (types_members
// dmgTYPE): dscDESC base@0x00 (12B), state@0x0C, name@0x10 — the remaining fields (id@0x14,
// phys@0x18 dmgTYPE_PHYS, shake*@0x30.., sensRec@0x44) are the next frontier and left unmodelled
// (the type is only ever handled by pointer here).

struct dmgTYPE : dscDESC {
    apSTATE_T<unsigned long> state; // 0x0C
    dsSTRID                  name;  // 0x10 damage-type name id
};
