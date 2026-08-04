#pragma once
// ws-engine ai08: a resolved strike candidate — the concrete (weapon, descriptor, vars) triple
// plus a scoring weight, used transiently while a weapon module recomputes strike variables.
// DB-verified layout (types_members aiSTRIKE): vars@0, wpn@4, desc@8, weight@0xC — size 16.

struct aiS_VARS;   // ai08 — per-strike cached vars (fwd)
struct wpnWEAPON;  // wpn — a single equipped weapon (fwd)
struct wpnS_DESC;  // wpn — weapon strike descriptor (fwd)

struct aiSTRIKE {
    aiS_VARS        *vars;   // 0x00 per-strike cached vars
    wpnWEAPON       *wpn;    // 0x04 owning weapon
    const wpnS_DESC *desc;   // 0x08 strike descriptor
    float            weight; // 0x0C scoring weight
};
