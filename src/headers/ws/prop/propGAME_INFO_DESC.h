#pragma once
#include "propCONTAINER_DESC.h"
// ws-engine prop: descriptor for a propGAME_INFO (the per-actor game-info property). A propGAME_INFO
// is itself a propCONTAINER, so its descriptor derives propCONTAINER_DESC. Boundary subset — only
// the runtime-type static consumed by aiBRAIN_IFACE::propGameInfo / propBASE::GetProperty is modeled
// here; the full descriptor layout is the next frontier.

struct propGAME_INFO_DESC : propCONTAINER_DESC {
    // DB-verified static (?TYPE_ID@propGAME_INFO_DESC@@2VdsTYPE_ID@@A @ 0x84314F4C) — this class's
    // runtime-type bitmask, tested by propGameInfo to recognise a game-info property.
    static dsTYPE_ID TYPE_ID;
};
