#pragma once
#include "../gs/gsGEOM_ANCHOR.h"
// ws-engine fsm: callback param block a weapon FSM raises to request the AI's current weapon target.
// DB-verified layout (types_members fsmWPN_TGT_INFO): fsmCB_PARAM_BASE base@0 (empty), wpn@0,
// tgt@4 (gsGEOM_ANCHOR,60), customPjlSpeed@0x40, posTip@0x44 — size 72.

struct wpnWEAPON; // wpn — a single equipped weapon (fwd)
struct m3dV;      // ws m3d — 3-vector (fwd)

struct fsmWPN_TGT_INFO {
    wpnWEAPON    *wpn;          // 0x00 requesting weapon
    gsGEOM_ANCHOR tgt;          // 0x04 target anchor
    float         customPjlSpeed;// 0x40
    m3dV         *posTip;       // 0x44 optional muzzle-tip override

    // ?Set@fsmWPN_TGT_INFO@@... — set the target to world position `pos`.
    // boundary — body external to this batch.
    void Set(const m3dV *pos);
};
