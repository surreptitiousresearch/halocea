/* ?GetMsg@MSG_PROCESSOR_BASE@con@@QAAXAAUCON_MSG@2@@Z @0x8309B268 */
#include "MSG_PROCESSOR_BASE.h"
#include "../ds/ds_assert_boundary.h"

namespace con {

// 0x8309B268 -- pop the front message out of receQueue into `msg`. The decompile shows a
// raw three-field (pData/nElem/allocated) swap between msg->data and the front element's
// data rather than a copy-then-erase; reconstructed here as an explicit dsVECTOR field swap
// (equivalent to CON_MSG::data.Swap) to keep the transfer allocation-free, then the emptied
// front slot is Erased.
void MSG_PROCESSOR_BASE::GetMsg(CON_MSG &msg)
{
    lock.Lock(nullptr, 0);

    if (!IGNORE_STRONG_ASSERT && receQueue.nElem <= 0) {
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            507,
            "nElem",
            receQueue.nElem);
    }
    msg.id = receQueue.pData->id;

    if (!IGNORE_STRONG_ASSERT && receQueue.nElem <= 0) {
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            507,
            "nElem",
            receQueue.nElem);
    }

    // Swap msg.data <-> receQueue.pData[0].data field-by-field (pData/nElem/allocated).
    CON_MSG &front = *receQueue.pData;
    unsigned char *tmpData      = msg.data.pData;
    msg.data.pData              = front.data.pData;
    front.data.pData            = tmpData;
    int tmpNElem                = msg.data.nElem;
    msg.data.nElem               = front.data.nElem;
    front.data.nElem            = tmpNElem;
    int tmpAllocated            = msg.data.allocated;
    msg.data.allocated           = front.data.allocated;
    front.data.allocated        = tmpAllocated;

    receQueue.Erase(0, 1);
    lock.Unlock(nullptr, 0);
}

} // namespace con
