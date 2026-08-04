#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// 0x8323CB30 — aiDEF_CDT_SYS::HaveResults. True when the in-flight request keyed by
// (pBrain, type, sgidx) exists and has produced its result.
bool aiDEF_CDT_SYS::HaveResults(const aiBRAIN *pBrain, aidr::DR_TYPE type, int sgidx) const
{
    keyREQ key;
    key.pSender = pBrain;
    if ( pBrain )
        key.pSenderDbg.SetPtr(const_cast<aiBRAIN *>(pBrain));
    key.typeReq = type;
    key.sgidx = sgidx;

    ReqConstIter it = dstReqs.Find(&key);
    return !it.IsDone() && it.value()->haveResult;
}
