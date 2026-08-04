#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ai/aiPLANNER.h"

// aiDOM_SPAWN::IsReadySpawn @ 0x83264638  ?IsReadySpawn@aiDOM_SPAWN@@AAAHXZ
// Gate that decides whether the domain may spawn this frame. Descriptor fields are read through
// the dscDESC[] stride (12B) view of spDesc.pointee (matching DoSpawn/IsActiveSpawn), resolving to:
//   desc[47].__vftable = nSpawnMax(0x234), desc[45].pBrand = timeOppSpawn(0x224),
//   desc[46].__vftable = timeAfterKill(0x228), desc[48].pBrand = nMaxSimultSameDom(0x248),
//   desc[48].__vftable = nKillSpawn(0x240).
int aiDOM_SPAWN::IsReadySpawn()
{
    unsigned int state = this->stateDom.state;
    if ((state & 1) == 0)          // domain not active
        return 0;
    if (this->nmbLocks > 0)
        return 0;
    if ((state >> 10) & 1)         // already completed
        return 0;

    dscDESC *desc = this->spDesc.pointee;
    int nSpawnMax = (int)desc[47].__vftable;
    if (nSpawnMax > 0 && this->nSpawned >= nSpawnMax)
        return 0;

    if (this->timeAfterSpawn < *(float *)&desc[45].pBrand
        || this->timeAfterKillCur < (float)(*(float *)&desc[46].__vftable - 0.000001f))
        return 0;

    int nMaxSimultSameDom = (int)desc[48].pBrand;
    if (nMaxSimultSameDom > 0)
    {
        int live = aiPlanner ? aiPlanner->CountLiveSpawnedFrom((gsDOM_SPAWN *)this) : 0;
        if (live < nMaxSimultSameDom)
        {
            int nKillSpawn = (int)desc[48].__vftable;
            if (nKillSpawn <= 1 || this->isBunchInProgress)
                return 1;
            if (live <= nMaxSimultSameDom - nKillSpawn)
            {
                this->isBunchInProgress = 1;
                return 1;
            }
        }
        return 0;
    }
    return 1;
}
