#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "apCOUNTER_UNIT.h"
// ws-engine ap (platform): a named runtime performance counter, owning one or more
// display "units" (gauges). Polymorphic base (has a vtable).
// DB-verified layout (types_members apCOUNTER) — size 32.

struct apCOUNTER_vtbl; // boundary — ap subsystem vtable

typedef struct apCOUNTER {
    static apCOUNTER_vtbl vftable; // `apCOUNTER::`vftable'' -- boundary, not decompiled

    apCOUNTER_vtbl            *__vftable;    // 0x00
    dsTSTRING<char>           name;          // 0x04 counter name
    dsVECTOR<apCOUNTER_UNIT, 8> counterUnits; // 0x08 display units
    unsigned int              id;            // 0x1C

    // 0x82719D00 — construct with `_name` and a '/'-delimited `unitsNames` list (e.g.
    // "calls", "time/calls"); each token becomes one display unit in `counterUnits`, then
    // registers this counter with the apCOUNTER_MANAGER singleton.
    apCOUNTER(const char *_name, const char *unitsNames);

    // 0x82715CE8 — count the units whose state matches `atState` (a state-bit mask). A unit
    // marked with the 0x40 "late-added" bit is also treated as carrying bit 0x02.
    int GetNUnits(unsigned char atState) const;

    // ?GetValue@apCOUNTER@@QBAMH@Z — read the current value of display unit `unit`. const.
    float GetValue(int unit) const;
    // ?SetValue@apCOUNTER@@QAAXHM@Z — overwrite the value of display unit `unit`.
    void SetValue(int unit, float value);
    // ?AddValue@apCOUNTER@@QAAXHM@Z — add to the value of display unit `unit`.
    void AddValue(int unit, float value);
} apCOUNTER;
