#pragma once
#include "../ds/dsSTRID.h"
#include "propGAME_INFO_TEAM.h"
// ws-engine prop: per-actor game-info block. Only the team-assignment surface consumed by the
// ai08 teams batch is modelled here; the full layout is a boundary (reversed in a later wave).
// DB-verified partial layout (types_members propGAME_INFO): propSYNCABLE base@0x00 (44),
// teamID@0x2C, isCursorColorizeTarget@0x34 — size 56.

struct propGAME_INFO {
    unsigned char      _pad0[0x2C];   // 0x00 boundary — the propSYNCABLE base, not yet reversed
    propGAME_INFO_TEAM teamID;        // 0x2C the actor's assigned team handle
    bool               isCursorColorizeTarget; // 0x34

    // ?SetTeamID@propGAME_INFO@@QAAXABVdsSTRID@@@Z @0x82DA8100 — assign this actor's team id. boundary.
    void SetTeamID(const dsSTRID &team);

    // Virtual (propGAME_INFO vtbl+0x84) — the tracked actor's body height. Used by
    // aiWATCHER::FillHeightsWithProp.  boundary.
    float GetHeight() const;
    // Virtual (propGAME_INFO vtbl+0x88) — the tracked actor's minimum flesh height. boundary.
    float GetMinHeightFlesh() const;
};
