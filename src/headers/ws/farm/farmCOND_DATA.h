#pragma once
#include "../ap/apSTATE_T.h"
#include "../ds/dsSTRID.h"
// ws-engine farm: per-unit farming condition data (create/destroy radii + group binding).
// DB-verified layout (types_members farmCOND_DATA): state@0, radCreate@4, radDestroy@8,
// farmGroup@12 — size 16.

struct farmCOND_DATA {
    apSTATE_T<unsigned long> state;      // 0x00
    float                    radCreate;  // 0x04 create radius
    float                    radDestroy; // 0x08 destroy radius
    dsSTRID                  farmGroup;  // 0x0C farm-group name id
}; // 16 bytes
