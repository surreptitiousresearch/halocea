#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsDEQUE.h"
#include "../m3d/m3dBOX.h"
#include "scrSCORCH_MARK_STAT.h"
#include "scrSCORCH_MARK_STAT_CD.h"
// ws-engine scorch: one spatial voxel of the scorch grid -- holds the static scorch batches and
// marks that fall inside it. DB-verified layout (types_members scrVOX) -- size 104.

struct scrBATCH_STAT; // scrBATCH_STAT.h -- boundary (dsVECTOR element, pointer only)

// DB-verified layout (types_members scrVOX_VISIBILITY): minDistToCam@0, isVisible@4 — size 8.
typedef struct scrVOX_VISIBILITY {
    float         minDistToCam; // 0x00 closest camera distance this frame
    bool          isVisible;    // 0x04
    unsigned char _pad05[3];    // 0x05
} scrVOX_VISIBILITY;

typedef struct scrVOX {
    dsVECTOR<scrBATCH_STAT, 8>          batches;           // 0x00 (20)
    dsVECTOR<scrSCORCH_MARK_STAT, 8>    scorchmarks;       // 0x14 (20)
    m3dBOX                              bbox;              // 0x28 (24)
    scrVOX_VISIBILITY                   visibility;        // 0x40 (8)
    dsDEQUE<scrSCORCH_MARK_STAT_CD>     scorchmarksFarmed; // 0x48 (32)
} scrVOX;                                                  // 104 bytes
