#include "msgSYSTEM.h"
#include "msgADDR.h"
#include "msgRES.h"

// ws-engine msg debug globals: record the message currently being dispatched (for the debugger).
extern msgADDR *pDbgRecv; // ?pDbgRecv@@3PAVmsgADDR@@A
extern msgADDR *pDbgSend; // ?pDbgSend@@3PAVmsgADDR@@A
extern int      dbgMsg;   // ?dbgMsg@@3HA
extern void    *pDbgInfo; // ?pDbgInfo@@3PAXA

// msgSYSTEM::Dispatch @ 0x82556F60
// Run one message through `pRecv`'s dispatch chain. Filter it out (NOT_SENT) when: it is an init
// message (1) but the address is already active; the address is frame-skipped/suspended (0x10); or
// the address has a terminate posted (0x8) and this is not a lifecycle message (2/4). Otherwise
// record it for the debugger, latch the active/terminating state for init/terminate messages, and
// call PreProcessMsg / ProcessMsg / PostProcessMsg. PreProcess consuming the message, or a
// ProcessMsg result other than CONTINUE, short-circuits the chain.
msgDSP_RES msgSYSTEM::Dispatch(msgADDR *pRecv, int msg, void *pInfo, msgADDR *pSend)
{
    if (msg == 1 && (pRecv->stateAddr.state & 1) != 0)
        return MSG_DSP_RES_NOT_SENT;
    if ((pRecv->stateAddr.state & 0x10) != 0)
        return MSG_DSP_RES_NOT_SENT;
    if ((pRecv->stateAddr.state & 8) != 0 && msg != 2 && msg != 4)
        return MSG_DSP_RES_NOT_SENT;

    pDbgRecv = pRecv;
    pDbgSend = pSend;
    dbgMsg   = msg;
    pDbgInfo = pInfo;

    if (msg == 1)
        pRecv->stateAddr.state |= 1u;
    if (msg == 2)
        pRecv->stateAddr.state |= 2u;

    if (pRecv->__vftable->PreProcessMsg(pRecv, msg, pInfo, pSend))
        return MSG_DSP_RES_SENT;

    msgRES processResult = pRecv->__vftable->ProcessMsg(pRecv, msg, pInfo, pSend);
    if (processResult != MSG_RES_CONTINUE)
        return processResult == MSG_RES_ERROR ? MSG_DSP_RES_ERROR : MSG_DSP_RES_SENT;

    pRecv->__vftable->PostProcessMsg(pRecv, msg, pInfo, pSend);
    return MSG_DSP_RES_SENT;
}
