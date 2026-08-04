#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
// ws-engine farm: a named group of farm units. DB-verified layout (types_members farmGROUP):
// name@0, unitList@4 — size 24.

struct farmUNIT;

struct farmGROUP {
    dsSTRID                 name;     // 0x00 group name id
    dsVECTOR<farmUNIT *, 8> unitList; // 0x04 member units
}; // 24 bytes
