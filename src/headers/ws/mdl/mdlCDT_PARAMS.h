#pragma once
#include "mdlCDT_SETTINGS.h"
// ws-engine mdl: per-frame collision-detection command params inside mdlCMD2BODY. DB-verified
// layout (types_members mdlCDT_PARAMS): dirFloorPlace@0 .. wpnType@0x78, cdtSettings@0x7C — size 0xA0.
// Only cdtSettings is typed here; the leading region stays byte-accurate opaque (next frontier).

struct mdlCDT_PARAMS {
    unsigned char   _pad00[0x7C]; // 0x00 dirFloorPlace/flags/wpnOrgDirTip/wpnType (opaque)
    mdlCDT_SETTINGS cdtSettings;  // 0x7C collision-detection tuning (size 0x24, ends 0xA0)
};
