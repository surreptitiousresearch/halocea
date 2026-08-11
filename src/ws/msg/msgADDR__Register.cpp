#include "msgADDR.h"
#include "msgSYSTEM.h"
#include "msgDATA.h"
#include "../ds/ds_assert_boundary.h"

// ws-engine gs boundary globals / queries (not reversed here).
extern unsigned long gsAppState;     // ?gsAppState@@3KA — global app-state bitmask
extern unsigned long gsMsgGetMode(); // ?gsMsgGetMode@@YAKXZ — current message-system mode

// msgADDR::Register @ 0x8255D808
// Register this address with the global message system. First deliver the init message (1); if
// that errors, back the address out of the queues (asserting it is not already terminating) and
// report failure. On success, replay whatever activation/render/spawn messages the current app
// state calls for, link the address into the priority buckets, tell subscribers it appeared, and
// push the current mode to it. Returns whether registration succeeded.
bool msgADDR::Register(msgDATA *pInitData)
{
    if (msgSystem.SendMsg(this, 1, pInitData, nullptr) == MSG_DSP_RES_ERROR) {
        msgSystem.RemoveFromQueues(this);
        if (!IGNORE_STRONG_ASSERT
            && ((this->stateAddr.state & 8) != 0 || (this->stateAddr.state & 2) != 0))
            static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
                "!stateAddr.Is(MSG_ADDR_ST_TERM_POSTED) && !stateAddr.Is(MSG_ADDR_ST_TERMED)",
                "D:\\Projects\\code\\common\\src.sys\\gm_sys\\msg\\msgaddr.cpp",
                83,
                empty_string);
        return false;
    }

    if ((gsAppState & 2) != 0)
        msgSystem.SendMsg(this, 1007, nullptr, nullptr);
    if ((gsAppState & 0x2000) != 0)
        msgSystem.SendMsg(this, 1012, nullptr, nullptr);
    if ((gsAppState & 2) != 0)
        msgSystem.SendMsg(this, 1009, nullptr, nullptr);

    msgSystem.InsertAddr(this);
    msgSystem.NotifySubscribers(3, nullptr, this);

    if (gsMsgGetMode())
        this->__vftable->OnChangeMode(this, 0, gsMsgGetMode());
    return true;
}
