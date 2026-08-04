#pragma once
#include "../m3d/m3dV.h"
// ws-engine ia sound subsystem: the resolved 3-D source description handed to the sound engine when
// a play request is issued. DB-verified layout (types_members iaSND_SOURCE_DATA) — size 20:
//   pObjOwner@0, startPos@4 (m3dV), startPosValid@0x10.

struct objOBJ; // ws obj — owning object (fwd, boundary pointer)

struct iaSND_SOURCE_DATA {
    objOBJ *pObjOwner;     // 0x00 owning object (null when a raw world position is used)
    m3dV    startPos;      // 0x04 explicit world-space start position
    bool    startPosValid; // 0x10 set once startPos has been supplied

    // 0x82A4EB88 (?SetStartPos@iaSND_SOURCE_DATA@@QAAXPAUm3dV@@@Z) — copy `pos` into startPos and
    // mark it valid. boundary.
    void SetStartPos(m3dV *pos);
};
