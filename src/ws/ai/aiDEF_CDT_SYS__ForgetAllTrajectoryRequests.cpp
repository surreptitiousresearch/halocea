#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiDEF_CDT_SYS::ForgetAllTrajectoryRequests @ 0x8323F010
// ?ForgetAllTrajectoryRequests@aiDEF_CDT_SYS@@UAAXPBVaiBRAIN@@@Z
//
// Drop every trajectory request belonging to `pBrain` from both the accumulation map (srcReqs) and
// the in-flight map (dstReqs). "Forgetting" is done by re-keying each matching request to a fresh,
// never-matched sub-group index (counted up from above the current maximum) and erasing the
// original entry, so no future lookup by the original (sender, DR_TRAJECTORY, sgidx) key hits it.

namespace {

// File-local helper (template ??$ForgetMapRequests@... @ 0x8323E478, instantiated once for the
// ReqMap type). The map key type is aiDEF_CDT_SYS::keyREQ and the value type aidr::DR *.
void ForgetMapRequests(aiDEF_CDT_SYS::ReqMap &map, const aiBRAIN *pBrain)
{
    typedef aiDEF_CDT_SYS::keyREQ keyREQ;
    typedef aiDEF_CDT_SYS::ReqIter ReqIter;

    // Pass 1 — the smallest sub-group index guaranteed not to collide with any live trajectory
    // request of any brain. Starts at a large sentinel so re-keyed entries never re-match below.
    int freeSgidx = 1000000;
    for (ReqIter it(map.Begin()); !it.IsDone(); ++it)
    {
        const keyREQ &k = it.key();
        if (k.pSender == pBrain && k.typeReq == aidr::DR_TRAJECTORY && k.sgidx >= freeSgidx)
            freeSgidx = k.sgidx + 1;
    }

    // Pass 2 — re-key pBrain's trajectory requests to fresh indices and erase the originals.
    for (ReqIter it(map.Begin()); !it.IsDone(); )
    {
        const keyREQ &k = it.key();
        if (pBrain == k.pSender && k.typeReq == aidr::DR_TRAJECTORY && k.sgidx < 1000000)
        {
            keyREQ nk;
            nk.pSender = k.pSender;
            nk.pSenderDbg.SetPtr(const_cast<aiBRAIN *>(k.pSender));
            nk.typeReq = k.typeReq;
            nk.sgidx = freeSgidx++;
            map.InsertNew(&nk, it.value());
            it = map.Erase(it);
        }
        else
        {
            ++it;
        }
    }
}

} // namespace

void aiDEF_CDT_SYS::ForgetAllTrajectoryRequests(const aiBRAIN *pBrain)
{
    ForgetMapRequests(srcReqs, pBrain);
    ForgetMapRequests(dstReqs, pBrain);
}
