#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiDOM_SPAWN::SelectSpawnPos @ 0x83265AA0  ?SelectSpawnPos@aiDOM_SPAWN@@AAAXPAUm3dV@@@Z
// Recompute the spawn-point weights (random vs even, chosen by desc.isFullRandom = desc[49].ref,
// 0x250), then output the position of the lowest-weight point.
void aiDOM_SPAWN::SelectSpawnPos(m3dV *pos)
{
    if (*(int *)&this->spDesc.pointee[49].ref)
        this->CalcWeightRandom();
    else
        this->CalcWeightEven();

    int best = 0;
    float minWeight = this->pointsList[0].weight;
    for (int i = 1; i < this->pointsList.nElem; ++i)
    {
        if (this->pointsList[i].weight < minWeight)
        {
            minWeight = this->pointsList[i].weight;
            best = i;
        }
    }
    *pos = this->pointsList[best].pos;
}
