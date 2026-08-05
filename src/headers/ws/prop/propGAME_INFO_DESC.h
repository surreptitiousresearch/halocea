#pragma once
#include "../DEFAULT_CTOR.h"
#include "../ds/dsTSTRING.h"
#include "propSYNCABLE_DESC.h"
#include "propGAME_INFO_TEAM.h"
// ws-engine prop: descriptor for a propGAME_INFO (the per-actor game-info property).
// DB-verified layout (types_members propGAME_INFO_DESC): propSYNCABLE_DESC base@0 (52),
// isEntSslClassInited@0x34, nameTeamCached@0x38, teamID@0x3C, isCursorColorizeTarget@0x44,
// isAimAssistTarget@0x45, killerType@0x48, height@0x4C, minHeightFlesh@0x50 — size 84.
//
// DEVIATION: this header used to derive propCONTAINER_DESC and declare no members, which sized
// the block 92. The DB's base is propSYNCABLE_DESC, matching propGAME_INFO's own propSYNCABLE
// base (propENT 40 + mp::MSG_IFACE 4 = the 44-byte prefix in propGAME_INFO.h).

struct propGAME_INFO_DESC : propSYNCABLE_DESC {
    DEFAULT_CTOR<bool>  isEntSslClassInited;    // 0x34 lazy-init flag for the ssl class link
    dsTSTRING<char>     nameTeamCached;         // 0x38 last team name parsed, cached
    propGAME_INFO_TEAM  teamID;                 // 0x3C default team handle
    bool                isCursorColorizeTarget; // 0x44
    bool                isAimAssistTarget;      // 0x45
    dsTSTRING<char>     killerType;             // 0x48
    float               height;                 // 0x4C body height (propGAME_INFO::GetHeight)
    float               minHeightFlesh;         // 0x50 (propGAME_INFO::GetMinHeightFlesh)

    // DB-verified static (?TYPE_ID@propGAME_INFO_DESC@@2VdsTYPE_ID@@A @ 0x84314F4C) — this class's
    // runtime-type bitmask, tested by propGameInfo to recognise a game-info property.
    static dsTYPE_ID TYPE_ID;
};
