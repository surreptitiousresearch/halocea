#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// 0x8323CA48 — aiDEF_CDT_SYS::HaveRequest. True when a request keyed by (pBrain, type, sgidx) is
// present in either the accumulating (srcReqs) or the in-flight (dstReqs) map. The key's WEAK_PTR
// debug field takes/releases a handle reference across its lifetime (the osLockedIncrement /
// osLockedDecrement + delete in the disassembly are the WEAK_PTR ctor/dtor inlined).
bool aiDEF_CDT_SYS::HaveRequest(const aiBRAIN *pBrain, aidr::DR_TYPE type, int sgidx) const
{
    keyREQ key;
    key.pSender = pBrain;
    if ( pBrain )
        key.pSenderDbg.SetPtr(const_cast<aiBRAIN *>(pBrain));
    key.typeReq = type;
    key.sgidx = sgidx;

    bool inSrc = !srcReqs.Find(&key).IsDone();
    bool inDst = !dstReqs.Find(&key).IsDone();
    return inSrc || inDst;
}
