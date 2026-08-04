#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiDOM_SPAWN::CalcWeightEven @ 0x83265A08  ?CalcWeightEven@aiDOM_SPAWN@@AAAXXZ
// Seed every spawn point with a tiny random jitter (to break ties), then layer on the proximity
// bias from existing AI units and from players so spawns favour populated areas.
void aiDOM_SPAWN::CalcWeightEven()
{
    for (int i = 0; i < this->pointsList.nElem; ++i)
        this->pointsList[i].weight = m3dRndGen.RndFloatRange(0.0f, 0.000001f);

    this->AddWeightsByExisting();
    this->AddWeightsByPlayers();
}
