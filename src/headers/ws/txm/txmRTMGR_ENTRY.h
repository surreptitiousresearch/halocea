#pragma once
#include "txmTEXTURE_PTR.h"
// ws-engine txm: one render-target-manager slot in txmMANAGER::rtMgrTargets.
// DB-verified layout (types_members txmRTMGR_ENTRY): pTex@0 (txmTEXTURE_PTR), flags@4,
// in_use@8, locked@12, tag@16 (const char *) — size 20.

typedef struct txmRTMGR_ENTRY {
    txmTEXTURE_PTR pTex;   // 0x00
    unsigned int   flags;  // 0x04
    int            in_use; // 0x08
    int            locked; // 0x0C
    const char    *tag;    // 0x10
} txmRTMGR_ENTRY;
