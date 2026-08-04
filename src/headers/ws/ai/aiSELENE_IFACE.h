#pragma once
#include "aiSELENE_BASE.h"
// ws-engine ai08 — the generic "selene" (enemy-selection) interface: an aiSELENE_BASE carrying the
// owning brain. Hosts the "zero" vtable and is the base of the concrete aiSELENE arm. DB-verified
// layout (types_members aiSELENE_IFACE): aiSELENE_BASE@0, pBrain@4 — size 8.

struct aiBRAIN; // ai08 — per-unit AI brain   boundary (fwd)

// 0x83175690 (??0aiSELENE_IFACE@@QAA@PAVaiBRAIN@@@Z).
struct aiSELENE_IFACE : aiSELENE_BASE {
    aiBRAIN *pBrain; // 0x04 owning brain

    aiSELENE_IFACE(aiBRAIN *pBrain); // boundary ctor
};
