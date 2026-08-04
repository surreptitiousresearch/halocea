#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWAH_SYS_boundaries.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiWAH_SYS::UnRegisterActive(aiBRAIN* pBrain) @ 0x83172630
// ?UnRegisterActive@aiWAH_SYS@@QAAXPAVaiBRAIN@@@Z
//
// Remove every active-unit watcher tracking `pBrain`: clear the brain status back-pointers,
// unordered-erase the slot (swap with the tail, then Erase the tail), forgo the watcher, and
// keep scanning (the index is rewound so the swapped-in element is re-tested).
void aiWAH_SYS::UnRegisterActive(aiBRAIN *pBrain)
{
    for (int i = 0; i < arrUnits.nElem; ++i)
    {
        aiWATCHER_BRAIN *watcher = arrUnits[i];
        if (watcher && watcher->Is(reinterpret_cast<const entENTITY *>(pBrain)))
        {
            pBrain->st.pointee->peekerSelf = nullptr;
            // Debug poison value (0xDDDDDDDE) written to the duplicate back-pointer.
            pBrain->st.pointee->dbgDuplicatePeekerSelf =
                reinterpret_cast<aiWATCHER_BRAIN *>(-555819298);

            const int last = arrUnits.nElem - 1;
            arrUnits[i] = arrUnits[last];
            arrUnits[last] = nullptr;
            arrUnits.Erase(last, 1);
            --i;
            Forgo(reinterpret_cast<aiWATCHER *>(watcher));
        }
    }
}
