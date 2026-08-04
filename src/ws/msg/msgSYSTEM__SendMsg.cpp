#include "msgSYSTEM.h"
#include "msgADDR.h"

// msgSYSTEM::SendMsg @ 0x82557178
// Deliver `msg` to `pRecv`: reject if the recipient is not accepting messages (stateAddr bit
// 0x2); otherwise dispatch when it does not restrict to lifecycle events (bit 0x8 clear) or when
// this is itself a lifecycle event (msg 2 or 4).
msgDSP_RES msgSYSTEM::SendMsg(msgADDR *pRecv, int msg, void *pInfo, msgADDR *pSend)
{
    // Original tests (cntlzw(state & 2) & 0x20) == 0, i.e. bit 0x2 is set.
    if ((pRecv->stateAddr.state & 0x2) != 0)
        return MSG_DSP_RES_NOT_SENT;

    // (cntlzw(state & 8) & 0x20) != 0  <=>  bit 0x8 is clear.
    if ((pRecv->stateAddr.state & 0x8) == 0 || msg == 2 || msg == 4)
        return this->Dispatch(pRecv, msg, pInfo, pSend);

    return MSG_DSP_RES_NOT_SENT;
}
