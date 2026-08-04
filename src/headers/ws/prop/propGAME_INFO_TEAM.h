#pragma once
#include "../ds/dsSTRID.h"
// ws-engine prop: a game-info team handle — a vtable pointer plus the interned team id.
// DB-verified layout (types_members propGAME_INFO_TEAM): __vftable@0, team@4 (dsSTRID) — size 8.

struct propGAME_INFO_TEAM_vtbl; // opaque boundary — only its address is taken

struct propGAME_INFO_TEAM {
    propGAME_INFO_TEAM_vtbl *__vftable; // 0x00
    dsSTRID                  team;      // 0x04 interned team id

    // Install this class's vtable. boundary.
    propGAME_INFO_TEAM();

    // ?EnsureValid@propGAME_INFO_TEAM@@... — normalise `team` (map an empty/unknown id to the
    // canonical neutral team). boundary — body external to this batch.
    void EnsureValid();

    static propGAME_INFO_TEAM_vtbl vftable; // `propGAME_INFO_TEAM::`vftable'' — boundary
};
