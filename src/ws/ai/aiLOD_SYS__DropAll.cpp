#include "../../headers/ws/ai/aiLOD_SYS.h"

// aiLOD_SYS::DropAll @ 0x83239D30
// ?DropAll@aiLOD_SYS@@QAAXXZ
//
// Reset the dynamic LOD state: drop every queued strike-performance-load sample, zero the cached
// load sum, and clear the mind-sparser permit.
void aiLOD_SYS::DropAll()
{
    queueLoads.Clear();
    cachedLoadSum = 0.0f;
    msAllowed.idx = 0;
    msAllowed.pb = nullptr;
}
