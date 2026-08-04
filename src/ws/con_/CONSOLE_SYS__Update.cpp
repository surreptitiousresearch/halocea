#include "CONSOLE_SYS.h"
#include "CONSOLE_CALLBACK.h" // full layout: pCb->Update()/UpdateAfterMessages() dispatched below
#include "../ds/ds_assert_boundary.h"

// con::CONSOLE_SYS::Update(int) @ 0x827EADD0
// Per-frame tick. `frame` == -1 means "keep the previously recorded frame number"
// (used by callers that only want to pump messages without advancing the frame
// counter). Runs every registered callback's Update, then pumps the inherited
// MSG_PROTOCOL, then runs every registered callback's UpdateAfterMessages.
//
// NOTE: the decompiler split the loop counter into two lockstep locals (v3/v5 and
// v6/v7) that are always equal (both start at 0 and increment together, bounded by
// the same listCb.nElem check) -- collapsed here to one index per loop.
void con::CONSOLE_SYS::Update(int frame)
{
    if (frame != -1)
        this->frameNmb = frame;

    for (int callback_index = 0; callback_index < this->listCb.nElem; ++callback_index)
    {
        if (!IGNORE_STRONG_ASSERT && (callback_index < 0 || callback_index >= this->listCb.nElem))
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                567,
                "idx", callback_index,
                "nElem", this->listCb.nElem);
        this->listCb.pData[callback_index].pCb->Update();
    }

    con::MSG_PROTOCOL::Update();

    for (int callback_index = 0; callback_index < this->listCb.nElem; ++callback_index)
    {
        if (!IGNORE_STRONG_ASSERT && (callback_index < 0 || callback_index >= this->listCb.nElem))
            STRONG_ASSERT2_HELPER::asserd<int, int>(
                "idx >= 0 && idx < nElem",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                567,
                "idx", callback_index,
                "nElem", this->listCb.nElem);
        this->listCb.pData[callback_index].pCb->UpdateAfterMessages();
    }
}
