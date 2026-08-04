#pragma once
#include "../m3d/m3dOBB.h"
// ws-engine nav (navGPH) boundary — the nav-graph blocker types passed across the navMANAGER
// interface. navBLOCK_OBB pairs an oriented-bounding-box volume with the anim instance that owns
// it (null for a static/anonymous volume). Passed by value in dsVECTOR<navGPH::navBLOCK_OBB,8>
// lists to navMANAGER::Block/UnblockByVolList.
// DB-verified layout (types_members navGPH::navBLOCK_OBB): vol@0 (m3dOBB, 60), pInstOwner@0x3C.

struct animINST; // anim instance (owner) — boundary (fwd)

namespace navGPH {

struct navBLOCK_OBB {
    m3dOBB    vol;        // 0x00 blocking volume
    animINST *pInstOwner; // 0x3C owning anim instance (null when unowned)
};

} // namespace navGPH
