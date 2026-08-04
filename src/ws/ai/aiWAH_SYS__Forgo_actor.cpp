#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiWAH_SYS::Forgo(iaIACTOR* actor) @ 0x83172190
// ?Forgo@aiWAH_SYS@@QAAXPAViaIACTOR@@@Z
//
// Find the holder whose watcher tracks `actor` and drop it (by index). No-op if absent.
void aiWAH_SYS::Forgo(iaIACTOR *actor)
{
    const int nElem = storage.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiWTC_HOLDER *holder = storage[i];
        if (holder->watcher->Is(actor))
        {
            Forgo(holder, i);
            return;
        }
    }
}
