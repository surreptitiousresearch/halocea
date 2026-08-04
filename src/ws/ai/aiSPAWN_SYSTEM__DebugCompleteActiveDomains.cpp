#include "../../headers/ws/ai/aiSPAWN_SYSTEM.h"
#include "../../headers/ws/ai/aiDOM_SPAWN.h"

// aiSPAWN_SYSTEM::DebugCompleteActiveDomains() @ 0x832A2198
// ?DebugCompleteActiveDomains@aiSPAWN_SYSTEM@@UAAXXZ (virtual)
// Force every currently-active spawn domain to its completed state (debug tool).
void aiSPAWN_SYSTEM::DebugCompleteActiveDomains()
{
    for (int i = 0; i < this->domList.nElem; ++i)
    {
        aiDOM_SPAWN *dom = (aiDOM_SPAWN *)this->domList[i];
        if (dom && dom->IsActiveSpawn())
            dom->MarkCompleted();
    }
}
