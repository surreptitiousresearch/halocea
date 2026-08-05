#include "msgSYSTEM.h"
#include "msgADDR.h"
#include "msgQUEUE.h"
#include "../ds/ds_assert_boundary.h"

// msgSYSTEM::PostMsg @ 0x82557238
// Queue `msg` for delivery to `pRecv` on the next frame. A terminate message (2) may only be
// posted to a live address (state bit 0x200); once posted it flips the term-posted bit (0x8) and
// the recipient is told a terminate is pending.
void msgSYSTEM::PostMsg(msgADDR *pRecv, int msg, msgADDR *pSend)
{
    if (!pRecv)
        return;

    if (msg == 2 && (pRecv->stateAddr.state & 0x200) == 0 && !IGNORE_STRONG_ASSERT)
        STRONG_ASSERT_DUMMY().Crash(
            "0",
            "D:\\Projects\\code\\common\\src.sys\\gm_sys\\msg\\msgaddr.cpp",
            414,
            empty_string);

    // Deliver only when the address accepts messages: neither the not-accepting bit (0x2) nor the
    // term-posted bit (0x8) is set.
    if ((pRecv->stateAddr.state & 0xA) == 0) {
        this->msgQueue.PutMsg(pRecv, msg, nullptr, pSend);
        if (msg == 2) {
            pRecv->stateAddr.state |= 8u;
            pRecv->__vftable->NotifyPostTermMsg(pRecv, pSend);
        }
    }
}
