#pragma once
#include "dsPARAM.h"
// ws-engine ds: lightweight read view over a parameter array (count + pointer).
// DB-verified layout (types_members dsPARAM_LIST): length@0 (int), list@4 (dsPARAM*) — size 8.

typedef struct dsPARAM_LIST {
    int      length; // 0x00 number of parameters
    dsPARAM *list;   // 0x04 pointer to the first parameter

    // 0x823B3648 (?Find@dsPARAM_LIST@@QBAPBVdsPARAM@@VdsSTRID@@@Z) — linear scan for the parameter
    // named `id`; returns null if not found. const (QBA). Body external to this batch.
    const dsPARAM *Find(dsSTRID id) const; // boundary
} dsPARAM_LIST;
