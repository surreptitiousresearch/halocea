#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWAH_boundaries.h"
#include "../../headers/ws/msg/msgTYPE_ITER.h"

// aiWAH_SYS::GetAllUnitsCnt @ 0x83170678
// ?GetAllUnitsCnt@aiWAH_SYS@@QBAHXZ
// Count every aiBRAIN endpoint registered with the message system that is currently instantiated
// (its entENTITY::pInst is non-null). Does not touch aiWAH_SYS state — the const `this` is unused.
// NOTE: the decompiler mislabels the per-brain field as `priorityInt`; disassembly reads +0x15C,
// which is entENTITY::pInst (animINST*).
int aiWAH_SYS::GetAllUnitsCnt() const
{
    int count = 0;
    for (msgTYPE_ITER<aiBRAIN> it(msgSystem.Front()); !it.IsDone(); ++it)
    {
        aiBRAIN *pBrain = it.Get();
        if (*reinterpret_cast<void *const *>(
                reinterpret_cast<const char *>(pBrain) + aiBRAIN_off_pInst))
            ++count;
    }
    return count;
}
