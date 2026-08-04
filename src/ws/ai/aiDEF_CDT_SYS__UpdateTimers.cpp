#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // m3dUpdateTimeField

// 0x8323CBF8 — aiDEF_CDT_SYS::UpdateTimers. Decay every in-flight request's destroy timer by `dt`.
// The disassembly walks the map's bucket chains inline; expressed here through the MAP iterator
// (Begin / IsDone / operator++), which is exactly that walk.
void aiDEF_CDT_SYS::UpdateTimers(float dt)
{
    for ( ReqIter it(dstReqs.Begin()); !it.IsDone(); ++it )
    {
        aidr::DR *dr = *it.value();
        m3dUpdateTimeField(&dr->destroyTimer, dt);
    }
}
