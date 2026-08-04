#pragma once
#include "scrSCORCH_MARK_BASE.h"
// ws-engine scorch: a static (baked-into-geometry) scorch mark.
// DB-verified layout (types_members scrSCORCH_MARK_STAT) -- size 96.

typedef struct scrSCORCH_MARK_STAT : scrSCORCH_MARK_BASE {
    unsigned int        splineScaleLastValue; // 0x50
    int                 iBatch;               // 0x54
    int                 objSourceFaceID;      // 0x58
    unsigned __int8     isSyncFarm;           // 0x5C
} scrSCORCH_MARK_STAT;
