#pragma once
#include <stdint.h>
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsSTRID.h"
#include "../m3d/m3dV.h"
// ws-engine scorch: shared header carried by every scorch mark (static or dynamic).
// DB-verified layout (types_members scrSCORCH_MARK_BASE) -- size 80.

struct scrSCORCH_DESC;  // scorch-descriptor payload (ref-counted) -- boundary (smart-ptr referent)
struct dsREF_COUNT;     // ../ds/dsREF_COUNT.h                        boundary
struct atkDISPATCHER;   // atk subsystem                             boundary (pointer only)

typedef struct scrSCORCH_MARK_BASE {
    dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > desc; // 0x00
    dsSTRID          classID;      // 0x04
    float            radius;       // 0x08
    float            rotAngle;     // 0x0C
    float            alphaScale;   // 0x10
    uint8_t  texIdx;       // 0x14
    m3dV             pos;          // 0x18 (12)
    m3dV             normal;       // 0x24 (12)
    int16_t          objSourceID;  // 0x30
    float            timeAdded;    // 0x34
    int              vtxIndStart;  // 0x38
    int              nVtx;         // 0x3C
    int              indIndStart;  // 0x40
    int              nInd;         // 0x44
    const atkDISPATCHER *pAtkDisp; // 0x48
    uint8_t  isSyncSL;     // 0x4C
} scrSCORCH_MARK_BASE;             // 80 bytes
