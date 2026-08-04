#pragma once
#include "dsSTRID.h"
#include "dsDATA.h"
// ws-engine ds: a named parameter — an interned id plus a type-erased value.
// DB-verified layout (types_members dsPARAM): id@0 (dsSTRID), data@4 (dsDATA) — size 12.

typedef struct dsPARAM {
    dsSTRID id;   // 0x00
    dsDATA  data; // 0x04

    // 0x8271F110 (??$GetVal@M@dsPARAM@@...) — decode this parameter's value as a float into
    // `*val`. PPC sret: the nominal `dsSTRID` return is a hidden result pointer in r3, shifting
    // `this` to r4 (see the project's sret gotcha) — modeled here as an ordinary return-by-value
    // dsSTRID that callers are expected to discard, matching every observed call site.
    dsSTRID GetVal(float *val) const; // boundary — external to this batch
} dsPARAM;
