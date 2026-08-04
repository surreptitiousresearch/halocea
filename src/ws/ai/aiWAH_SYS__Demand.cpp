#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ds/ds_new.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ds/dsVECTOR.h"


// aiWAH_SYS::Demand(iaIACTOR* actor) @ 0x83172048
// ?Demand@aiWAH_SYS@@QAAPAVaiWTC_HOLDER@@PAViaIACTOR@@@Z
//
// Return the holder already watching `actor` (bumping its counter), or create a new holder,
// push it into `storage`, and return it. Null actor yields null.
aiWTC_HOLDER *aiWAH_SYS::Demand(iaIACTOR *actor)
{
    if (!actor)
        return nullptr;

    const int nElem = storage.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiWTC_HOLDER *holder = storage[i];
        if (holder->watcher->Is(actor))
        {
            ++holder->counter;
            return holder;
        }
    }

    // No existing holder — allocate + construct one (tracked-new site preserved from the binary).
    void *raw = operator new(
        sizeof(aiWTC_HOLDER),
        "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_wah.cpp", 0x49);
    aiWTC_HOLDER *holder = raw ? new (raw) aiWTC_HOLDER(actor) : nullptr;
    storage.PushBack(holder);
    return holder;
}
