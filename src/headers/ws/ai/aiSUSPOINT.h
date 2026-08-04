#pragma once
// ws-engine ai08 — a single suspicion/attention point tracked by nsSUSCACHE.
// DB-verified layout (types_members aiSUSPOINT) — size 20 (0x14).

#include "aiSUSTYPE.h"
#include "../m3d/m3dV.h"

struct aiSUSPOINT {
    aiSUSTYPE type; // 0x00 classification
    short     wpID; // 0x04 associated nav waypoint id
    // 0x06..0x07 padding
    m3dV      pos;  // 0x08 world position
};
