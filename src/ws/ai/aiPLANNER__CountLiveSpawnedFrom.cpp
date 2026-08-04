#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"

struct gsDOM_SPAWN; // gs subsystem — spawn domain (compared by identity only)  boundary

// aiPLANNER::CountLiveSpawnedFrom(gsDOM_SPAWN*) const @ 0x831643C0
// Count the active unit watchers whose brain is still alive and was spawned from `dom`.
int aiPLANNER::CountLiveSpawnedFrom(gsDOM_SPAWN *dom) const
{
    int count = 0;
    for (int i = 0; i < this->wah.arrUnits.nElem; ++i)
    {
        aiWATCHER_BRAIN *watcher = this->wah.PeekActive(i);
        if (watcher && !watcher->IsDead() && watcher->GetDomSpawn() == dom)
            ++count;
    }
    return count;
}
