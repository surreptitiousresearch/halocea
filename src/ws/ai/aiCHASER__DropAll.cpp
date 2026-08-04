// aiCHASER::DropAll @ 0x8320BF20
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCHASER_boundary.h" // susCache
#include "../../headers/ws/ai/nsSUSCACHE.h"

// Empty every cached suspicion point, drop all caches, and reset the cache cursor.
void aiCHASER::DropAll()
{
    for (int i = 0; i < susCache.nElem; ++i)
        susCache[i].pts.nElem = 0;

    susCache.Clear();
    this->idxLastUpdatedCache = 0;
}
