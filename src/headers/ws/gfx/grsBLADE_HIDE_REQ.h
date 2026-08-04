#pragma once
#include "../m3d/m3dV.h"
// ws-engine grass subsystem: a deferred grass-blade hide/unhide request, queued on
// grsGRASS::hideRequest and consumed during the grass update.
// DB-verified layout (types_members grsBLADE_HIDE_REQ):
//   pos@0 (m3dV), radius@12 (float), searchInUnits@16 (bool), unhideAll@17 (bool).

typedef struct grsBLADE_HIDE_REQ {
    m3dV  pos;            // 0x00 center of the hide/unhide region
    float radius;         // 0x0C region radius
    bool  searchInUnits;  // 0x10 interpret radius/pos in grass-unit space
    bool  unhideAll;      // 0x11 unhide every blade (ignore pos/radius)
} grsBLADE_HIDE_REQ;
