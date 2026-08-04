#pragma once
#include "../ds/dsFIXED_BIT_ARRAY.h"
// ws-engine ctrl: a unit's property/state bit cache — a fixed 12-cell (384-bit) bit-array indexed by
// ctrlPROP_ID. DB-verified layout (types_members ctrlPROP_LIST): props@0 dsFIXED_BIT_ARRAY<12>,
// size 0x30 (48 bytes = 12 x 32-bit cells). Canonical shared home (promoted from the ai08 boundary
// subsets so aiBODY_IFACE::GetRefPropCache can return a single agreed type).

struct ctrlPROP_LIST {
    dsFIXED_BIT_ARRAY<12> props; // 0x00 (48 bytes = 12 x 32-bit cells)
};
