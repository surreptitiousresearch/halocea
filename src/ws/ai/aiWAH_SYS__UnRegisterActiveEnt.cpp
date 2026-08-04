#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/ai/aiWAH_boundaries.h" // IGNORE_STRONG_ASSERT / STRONG_ASSERT_DUMMY / byte_8200155A

// aiWAH_SYS::UnRegisterActiveEnt @ 0x83171960
// ?UnRegisterActiveEnt@aiWAH_SYS@@QAAXPAViaIACTOR@@@Z
// Remove the entity watcher tracking `actor`. If a dedicated entity watcher is in arrEnts, clear
// its foe flag, erase it and destroy it. Otherwise (no arrEnts entry) walk the holders and clear
// the foe flag on any holder-side entity watcher that still points at `actor`.
void aiWAH_SYS::UnRegisterActiveEnt(iaIACTOR *actor)
{
    if (!IGNORE_STRONG_ASSERT && !actor)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "actor",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_wah.cpp",
            1012, byte_8200155A);

    int found = -1;
    int nElem = arrEnts.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        if (arrEnts[i]->Is(actor))
        {
            found = i;
            break;
        }
    }

    if (found < 0)
    {
        // No dedicated arrEnts watcher: clear the foe flag on any matching holder watcher.
        int nHolders = storage.nElem;
        for (int i = 0; i < nHolders; ++i)
        {
            aiWATCHER *watcher = storage[i]->watcher;
            if (!IGNORE_STRONG_ASSERT && !watcher)
                STRONG_ASSERT_DUMMY::Crash(nullptr, "hld->Peek()",
                    "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_wah.cpp",
                    1033, byte_8200155A);

            if (watcher->IsEntity() && watcher->Is(actor))
            {
                aiWATCHER_ENT *entWatcher = reinterpret_cast<aiWATCHER_ENT *>(watcher);
                if (entWatcher->mayBeFoe)
                    entWatcher->mayBeFoe = false;
            }
        }
    }
    else
    {
        aiWATCHER_ENT *entWatcher = arrEnts[found];
        entWatcher->mayBeFoe = false;
        arrEnts.Erase(found, 1); // (inlined memmove + count-- in the original; equivalent)
        aiWATCHER::Destroy(reinterpret_cast<aiWATCHER *>(entWatcher));
    }
}
