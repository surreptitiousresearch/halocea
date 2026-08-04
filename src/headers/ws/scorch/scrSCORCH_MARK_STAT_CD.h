#pragma once
#include "scrSCORCH_MARK_BASE_CD.h"
// ws-engine scorch: compact static scorch mark stored in scrVOX::scorchmarksFarmed.
// DB-verified layout (types_members scrSCORCH_MARK_STAT_CD) -- size 56.

typedef struct scrSCORCH_MARK_STAT_CD : scrSCORCH_MARK_BASE_CD {
    int                    objSourceFaceID; // 0x34
} scrSCORCH_MARK_STAT_CD;
