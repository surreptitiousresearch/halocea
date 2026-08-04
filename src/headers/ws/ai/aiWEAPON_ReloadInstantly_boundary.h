#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
// Boundary surface for aiWEAPON::ReloadInstantly (a8_weapon.cpp @0x832B2D40): the wpn types the
// method reaches. Bodies external to this batch (the next frontier).

struct atkDSP_PARAMS; // wpn/atk — attack-dispatch parameter block (fwd)

// ?cmdWPN_RELOAD@@3VdsSTRID@@B — the interned "reload" weapon-FSM command id.  boundary.
extern const dsSTRID cmdWPN_RELOAD;

// fsmWPN_ATK_INFO — weapon FSM per-attack command parameter. DB-verified layout (types_members
// fsmWPN_ATK_INFO): fsmCB_PARAM_BASE base @0 (empty), atkID@0, params@4 — size 8. The empty base is
// omitted here (binary-equivalent: an empty base contributes no storage).
struct fsmWPN_ATK_INFO {
    int            atkID;   // 0x00 attack index
    atkDSP_PARAMS *params;  // 0x04 optional per-attack parameters
};

namespace wpn {
// wpn::SHOOTER_DESC — a shooter descriptor. DB-verified layout (types_members wpn::SHOOTER_DESC):
// attacks@0 (dsVECTOR<...,8>, 20), barrelDescs@0x14 (dsVECTOR<...,8>, 20), clipDescs@0x28
// (dsVECTOR<...,8>, 20) — size 0x3C. The exotic smart-pointer element types are the next frontier;
// only attacks.nElem is read here, so the vectors are modeled with a void* element (a dsVECTOR<T,8>
// is 20 bytes regardless of T, keeping pData/nElem/allocated at their DB offsets). boundary.
struct SHOOTER_DESC {
    dsVECTOR<void *, 8> attacks;     // 0x00
    dsVECTOR<void *, 8> barrelDescs; // 0x14
    dsVECTOR<void *, 8> clipDescs;   // 0x28
};
} // namespace wpn
