#pragma once
#include "CONST_ARRAY_m3dV.h"
// ws-engine ai08: aiVIS_REQ_RES — the line-of-sight request result copied back from the planner's
// def-CDT visibility system. DB-verified layout (types_members aiVIS_REQ_RES) — size 0xFC:
// percent@0, pointsFree@4, pointsIsects@0x80.

struct aiVIS_REQ_RES {
    float                percent;      // 0x00 resulting view-percent
    CONST_ARRAY_m3dV<10> pointsFree;   // 0x04 free-space sample points
    CONST_ARRAY_m3dV<10> pointsIsects; // 0x80 ray/geometry intersection points

    // Default construct (zeroes the arrays). boundary — body external to this batch.
    aiVIS_REQ_RES();
    // 0x832912E8 (??1aiVIS_REQ_RES@@QAA@XZ). boundary.
    ~aiVIS_REQ_RES();
};
