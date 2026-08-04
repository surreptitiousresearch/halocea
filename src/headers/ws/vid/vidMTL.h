#pragma once
#include "../ds/dsCONST_ARRAY.h"
#include "vidSHADER_DESC.h"
// ws-engine vid: a resolved material slot inside a vidOBJ_SPLIT's material table --
// the object-split-by-material analog of the mtl subsystem's mtlMTL (see mtlTBL.h).
// DB-verified layout (types_members vidMTL): name@0 (char[24]), sdrDescList@24
// (dsCONST_ARRAY<vidSHADER_DESC,5>, 24B) — size 48.

typedef struct vidMTL {
    char                                  name[24];       // 0x00 material name (NUL-terminated within the buffer)
    dsCONST_ARRAY<vidSHADER_DESC, 5>      sdrDescList;     // 0x18 per-pass shader descriptors
} vidMTL;
