#include "msgQUEUE.h"
#include "msgADDR.h"

// msgQUEUE::DeleteMsgByAddr @ 0x82AC4EA0
// Cancel every queued message that references `pAddr`: scan the still-undispatched tail of the
// current-frame buffer (from nextMsg on) and all of next frame's buffer, nulling any pRecv/pSend
// that equals `pAddr` so the envelope becomes a no-op.
void msgQUEUE::DeleteMsgByAddr(msgADDR *pAddr)
{
    for (int i = this->nextMsg; i < this->currQueue.nElem; ++i) {
        if (this->currQueue[i].pRecv == pAddr)
            this->currQueue[i].pRecv = nullptr;
        if (this->currQueue[i].pSend == pAddr)
            this->currQueue[i].pSend = nullptr;
    }
    for (int j = 0; j < this->nextQueue.nElem; ++j) {
        if (this->nextQueue[j].pRecv == pAddr)
            this->nextQueue[j].pRecv = nullptr;
        if (this->nextQueue[j].pSend == pAddr)
            this->nextQueue[j].pSend = nullptr;
    }
}
