#include "msgQUEUE.h"
#include "msgADDR.h"
#include "msgDATA.h"
#include "../ds/dsTYPE_ID.h"

// msgQUEUE::PutMsg @ 0x82AC5770
// Append a message envelope to next frame's queue: push a blank envelope, then fill it in via
// msgMSG::SetInfo (deep-copying `pInfo`'s payload when one is supplied).
void msgQUEUE::PutMsg(msgADDR *pRecv, int msg, msgDATA *pInfo, msgADDR *pSend)
{
    // Query the recipient's RTTI type-id; the result is discarded here (the original performs the
    // virtual TypeID call for its side effect / debug bookkeeping only). The machine call order is
    // TypeID(result, this); modeled through the header's (self, result) vtable prototype.
    dsTYPE_ID recvTypeID;
    pRecv->__vftable->TypeID(pRecv, &recvTypeID);

    msgMSG blank;
    blank.pRecv = nullptr;
    blank.pSend = nullptr;
    blank.msg = -1;
    blank.sizeInfo = 0;
    blank.pInfo = nullptr;
    blank.elapsedTime = 0.0f;
    this->nextQueue.PushBack(blank);

    if (pInfo) {
        int sizeStruct = pInfo->sizeStruct;
        msgMSG &slot = this->nextQueue.Back();
        slot.SetInfo(pRecv, msg, sizeStruct, pInfo, pSend, 0.0f);
    } else {
        int sizeStruct = 0;   /* no payload */
        msgMSG &slot = this->nextQueue.Back();
        slot.SetInfo(pRecv, msg, sizeStruct, nullptr, pSend, 0.0f);
    }
}
