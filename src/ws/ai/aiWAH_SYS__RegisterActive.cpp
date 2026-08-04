#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWAH_SYS_boundaries.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiWAH_SYS::RegisterActive(aiWTC_HOLDER* wh) @ 0x83172540
// ?RegisterActive@aiWAH_SYS@@QAAXPAVaiWTC_HOLDER@@@Z
//
// Register `wh`'s watcher as an active AI unit: push it into arrUnits, then wire the tracked
// brain's status back-pointer (peekerSelf / dbgDuplicatePeekerSelf) to this watcher. A null
// holder is a logic error (logged). The IGNORE_STRONG_ASSERT block only validates that the
// watcher's referent really is an aiBRAIN.
void aiWAH_SYS::RegisterActive(aiWTC_HOLDER *wh)
{
    if (!wh)
    {
        _apLog("~AI,Error~Planner: can't demand a watcher on register.");
        return;
    }

    aiWATCHER_BRAIN *watcher = reinterpret_cast<aiWATCHER_BRAIN *>(wh->watcher);
    arrUnits.PushBack(watcher);

    aiBRAIN *brain = watcher->brain.pHandle ? watcher->brain.pHandle->pPtr : nullptr;

    // Debug-only RTTI check that `brain` is an aiBRAIN (elided when asserts are ignored). The
    // dynamic-type test is ai08 boundary; represented here by the guarded assert path only.
    if (!IGNORE_STRONG_ASSERT && !aiBrainDynTypeIsBrain(brain))
        STRONG_ASSERT_DUMMY::Crash(nullptr, "ai->IsA<aiBRAIN>()",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_wah.cpp", 946,
            STRONG_ASSERT_INFO_STUB);

    brain->st.pointee->peekerSelf = watcher;
    brain->st.pointee->dbgDuplicatePeekerSelf = brain->st.pointee->peekerSelf;
}
