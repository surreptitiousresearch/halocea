#pragma once
#include "../os/osLOCK.h"
#include "../ds/dsVECTOR.h"
#include "rendLIGHT_SET.h"
// rend (renderer) global registry of live light sets. DB-verified layout
// (types_members rendLIGHT_SET_LIST): lock@0(52), lightSets@52(20) — total size 72 (0x48).
// Not polymorphic (no vtable slot).

typedef struct rendLIGHT_SET_LIST {
    osLOCK                          lock;      // 0x00 (52) guards lightSets
    dsVECTOR<rendLIGHT_SET *, 8>    lightSets; // 0x34 (52) registered light sets

    // 0x82ABDBB0 — register `pLS` in the live light-set list (asserts it is not already
    // present, then appends it under the lock).
    void OnCreateLightSet(rendLIGHT_SET *pLS);
} rendLIGHT_SET_LIST;
