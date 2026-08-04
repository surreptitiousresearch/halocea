#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"

// aiWAH_SYS::IsRegistered @ 0x83171BA8
// ?IsRegistered@aiWAH_SYS@@QAA_NPAVaiBRAIN@@@Z
// True if any active-unit watcher tracks `pBrain`. Also self-heals the list: on finding a second
// (duplicate) watcher for the same brain it swaps that slot with the last element and erases the
// tail, so at most one entry survives. The identity test is the watcher's Is() virtual (vtbl slot
// 0x4), which takes the brain's iaIACTOR subobject (at offset 0).
bool aiWAH_SYS::IsRegistered(aiBRAIN *pBrain)
{
    bool foundAny = false;
    for (int i = 0; i < arrUnits.nElem; ++i)
    {
        aiWATCHER *watcher = reinterpret_cast<aiWATCHER *>(arrUnits[i]);
        if (watcher && watcher->Is(reinterpret_cast<const iaIACTOR *>(pBrain)))
        {
            if (foundAny)
            {
                // Duplicate registration: unordered-remove this slot (swap with last, erase tail).
                int last = arrUnits.nElem - 1;
                arrUnits[i] = arrUnits[last];
                arrUnits[last] = nullptr;
                arrUnits.Erase(last, 1);
                --i;
            }
            foundAny = true;
        }
    }
    return foundAny;
}
