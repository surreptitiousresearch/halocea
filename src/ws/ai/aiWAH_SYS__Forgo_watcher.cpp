#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiWAH_SYS::Forgo(aiWATCHER* watcher) @ 0x83172128
// ?Forgo@aiWAH_SYS@@QAAXPAVaiWATCHER@@@Z
//
// Find the holder whose watcher is `watcher` and drop it (by index). No-op if absent.
void aiWAH_SYS::Forgo(aiWATCHER *watcher)
{
    const int nElem = storage.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiWTC_HOLDER *holder = storage[i];
        if (holder->watcher == watcher)
        {
            Forgo(holder, i);
            return;
        }
    }
}
