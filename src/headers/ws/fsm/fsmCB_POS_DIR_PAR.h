#pragma once
// ws-engine fsm: position/direction FSM-callback parameter block.
// DB-verified layout (types_members fsmCB_POS_DIR_PAR) — size 36 (0x24): fsmCB_PARAM_BASE base@0
// (empty), pos@0 (m3dV), dirZ@0xC (m3dV), dirY@0x18 (m3dV).
#include "../m3d/m3dV.h"
// fsmCB_PARAM_BASE — empty base (DB size 1, no members). boundary. Canonical body:
#include "fsmCB_PARAM_BASE.h"

struct fsmCB_POS_DIR_PAR : fsmCB_PARAM_BASE {
    m3dV pos;  // 0x00 target position
    m3dV dirZ; // 0x0C forward direction
    m3dV dirY; // 0x18 up direction
};
