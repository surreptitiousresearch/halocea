#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"

// 0x8323E840 — aiDEF_CDT_SYS::ProcessRequests. Promote every request accumulated this frame: emit
// its shoot rays (virtual), move it into the in-flight map, then clear the accumulation map.
void aiDEF_CDT_SYS::ProcessRequests()
{
    for ( ReqIter it(srcReqs.Begin()); !it.IsDone(); ++it )
    {
        aidr::DR *dr = *it.value();
        dr->__vftable->ProduceShootRays(dr);
        dstReqs.InsertNew(&it.key(), &dr);
    }
    srcReqs.Clear();
}
