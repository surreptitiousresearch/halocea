#pragma once
// ws-engine cdt: small/large collision-animation scene implementation. Only the base
// class layout and the one virtual entry point used by cdtCOLL_ANIM_SCN are modeled here —
// boundary (external to this batch).
// DB-verified layout (types_members cdtCOLL_ANIM_BASE_SCN): inherits cdtCOLL_ANIM_BASE (size 100).

// cdtCOLL_ANIM_BASE — DB-verified layout (types_members cdtCOLL_ANIM_BASE, size 100 / 0x64):
// cdtCOLL_DYN base@0 (0x58), nInvalidID@0x58, invalidIDList@0x5C, voxPosList@0x60.
#include "cdtCOLL_DYN.h"
#include "cdtCOLL_ANIM_VOX_POS.h"
struct cdtCOLL_ANIM_BASE : cdtCOLL_DYN {
    int                   nInvalidID;    // 0x58 count of pending invalidated instance ids
    int                  *invalidIDList; // 0x5C pending invalidated instance ids
    cdtCOLL_ANIM_VOX_POS *voxPosList;    // 0x60 per-instance cached voxel spans
}; // 100 bytes

struct cdtCOLL_ANIM_BASE_SCN : cdtCOLL_ANIM_BASE {
    // Virtual: invalidate cached collision-animation data for instance `instId`; `rebuild`
    // forces a rebuild (called with 0 for "invalidate only", or the delete flag when tearing
    // an instance down). boundary — not decompiled here.
    void Invalidate(int instId, int rebuild); // boundary (virtual)
};
