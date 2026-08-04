#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/ai/aiWAH_SYS_boundaries.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiWAH_SYS::RegisterActiveEnt(iaIACTOR* actor) @ 0x83172968
// ?RegisterActiveEnt@aiWAH_SYS@@QAAXPAViaIACTOR@@@Z
//
// Ensure an entity watcher exists for `actor`. If none of the current arrEnts watchers tracks it,
// create a watcher; keep it (marked foe-eligible) only when it is genuinely an entity watcher,
// otherwise destroy it.
void aiWAH_SYS::RegisterActiveEnt(iaIACTOR *actor)
{
    if (!IGNORE_STRONG_ASSERT && !actor)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "actor",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_wah.cpp", 985,
            STRONG_ASSERT_INFO_STUB);

    const int nElem = arrEnts.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (arrEnts[i]->Is(actor))
            return; // already tracked
    }

    aiWATCHER_ENT *watcher = reinterpret_cast<aiWATCHER_ENT *>(aiWATCHER::Create(actor));
    if (watcher->IsEntity())
    {
        arrEnts.PushBack(watcher);
        watcher->mayBeFoe = true;
    }
    else
    {
        aiWATCHER::Destroy(reinterpret_cast<aiWATCHER *>(watcher));
    }
}
