#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"

// aiWAH_SYS::ProcessFrame @ 0x83171740
// ?ProcessFrame@aiWAH_SYS@@QAAXM@Z
// Per-frame tick: forward `time` to Update() on every holder's watcher.
void aiWAH_SYS::ProcessFrame(float time)
{
    int nElem = storage.nElem;
    for (int i = 0; i < nElem; ++i)
        storage[i]->watcher->Update(time);
}
