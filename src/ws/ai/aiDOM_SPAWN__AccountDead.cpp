#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"

// aiDOM_SPAWN::AccountDead @ 0x83264540  ?AccountDead@aiDOM_SPAWN@@AAAXPAVaiBRAIN_IFACE@@@Z
// A unit spawned by this domain died: count it, log the tally, complete the domain if the kill
// quota is met, reset the kill timer and re-roll the next kill-delay from the descriptor.
// Descriptor floats are indexed through the float* view of spDesc.pointee:
//   descF[138]=timeAfterKill(0x228), descF[139]=delaySpawnMin(0x22C), descF[140]=delaySpawnMax(0x230).
void aiDOM_SPAWN::AccountDead(aiBRAIN_IFACE *pBrain)
{
    if (pBrain && (void *)pBrain->pDomSpawn == (void *)this)
    {
        ++this->nKill;
        _apLog("~Spawn~domSpawn '%s': nKill: %d; nKillCompl: %d",
               this->GetName().pBuffer->str, this->nKill, this->nKillComplete);

        if (this->nKillComplete > 0 && this->nKill >= this->nKillComplete)
            this->MarkCompleted();

        float *descF = (float *)this->spDesc.pointee;
        this->timeAfterKillCur = 0.0f;
        descF[138] = m3dRndGen.RndFloatRange(descF[139], descF[140]);
    }
}
