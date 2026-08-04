#pragma once
#include "../m3d/m3dV.h"
// ws-engine scorch: compact ("CD") scorch-mark header stored in the per-voxel farmed deque.
// DB-verified layout (types_members scrSCORCH_MARK_BASE_CD) -- size 52.

struct atkDISPATCHER; // atk subsystem -- boundary (pointer only)

typedef struct scrSCORCH_MARK_BASE_CD {
    float            rotAngle;    // 0x00
    float            alphaScale;  // 0x04
    float            radius;      // 0x08
    unsigned __int8  nameID;      // 0x0C
    m3dV             pos;         // 0x10 (12)
    m3dV             normal;      // 0x1C (12)
    float            timeAdded;   // 0x28
    unsigned __int8  texIdx;      // 0x2C
    __int16          objSourceID; // 0x2E
    const atkDISPATCHER *pAtkDisp; // 0x30
} scrSCORCH_MARK_BASE_CD;         // 52 bytes
