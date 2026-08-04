#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiDOM_SPAWN::CalcWeightRandom @ 0x832654B0  ?CalcWeightRandom@aiDOM_SPAWN@@AAAXXZ
// Assign every spawn point a fresh uniform random weight in [0, 100).
void aiDOM_SPAWN::CalcWeightRandom()
{
    for (int i = 0; i < this->pointsList.nElem; ++i)
        this->pointsList[i].weight = m3dRndGen.RndFloatRange(0.0f, 100.0f);
}
