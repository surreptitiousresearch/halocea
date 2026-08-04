#pragma once
#include "mdlCDT_PARAMS.h"
// ws-engine mdl: the per-frame command block a body FSM hands to its model. DB-verified layout
// (types_members mdlCMD2BODY): animParams@0, cdtParams@0xC4, csParams@0x164, ... — size 0x3E0.
// Only cdtParams is typed here; the surrounding blocks stay byte-accurate opaque (next frontier).

struct mdlCMD2BODY {
    unsigned char _pad00[0xC4];  // 0x00 animParams (opaque, size 0xC4)
    mdlCDT_PARAMS cdtParams;     // 0xC4 collision-detection params (size 0xA0, ends 0x164)
    unsigned char _pad164[0x3E0 - 0x164]; // 0x164 csParams/syncInfo/shared/type/stateAnim (opaque)
};
