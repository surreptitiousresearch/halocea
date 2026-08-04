#include "MSG_PROCESSOR_BASE.h"
#include "../ds/ds_assert_boundary.h"

namespace con {

// 0x8309B190 -- true when receQueue holds at least one live, non-terminator message.
// Drains any leading id==3 (terminator/response already consumed) entries first.
bool MSG_PROCESSOR_BASE::HaveMsg()
{
    // osLOCK::Lock/Unlock signature is (file, line); decompile passes (nullptr, 0) at
    // this call site (source location not captured here).
    lock.Lock(nullptr, 0);

    while (receQueue.nElem > 0) {
        if (!IGNORE_STRONG_ASSERT && receQueue.nElem <= 0) {
            STRONG_ASSERT2_HELPER::asserd<int>(
                "nElem > 0",
                "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
                507,
                "nElem",
                receQueue.nElem);
        }
        if (receQueue.pData->id != 3)
            break;
        receQueue.Erase(0, 1);
    }

    bool haveMsg = receQueue.nElem != 0;
    lock.Unlock(nullptr, 0);
    return haveMsg;
}

} // namespace con
