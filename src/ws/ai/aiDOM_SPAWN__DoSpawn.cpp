#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/ai/aiPLANNER.h"

// aiDOM_SPAWN::DoSpawn @ 0x83264748  ?DoSpawn@aiDOM_SPAWN@@AAAXXZ
// Descriptor fields are indexed through the dscDESC[] stride (12B): pointee[N].{refCount,pBrand}
// are the raw byte slots of aiDOM_SPAWN_DESC — kept as the observed reinterpret idiom.
void aiDOM_SPAWN::DoSpawn()
{
    dscDESC *desc = this->spDesc.pointee;

    // desc[45].pBrand <- rand[ desc[46].ref .. desc[46].pBrand ]  (all float-typed slots)
    *(float *)&desc[45].pBrand = m3dRndGen.RndFloatRange(
        *(float *)&desc[46].ref,
        *(float *)&desc[46].pBrand);

    int spawnCount = this->nSpawned ? 1 : *(int *)&this->spDesc.pointee[47].ref;
    if (spawnCount > 0)
    {
        do
        {
            this->SpawnSingle();
            --spawnCount;
        } while (spawnCount);
    }

    if ((int)desc[48].pBrand > 0)
    {
        int live = aiPlanner ? aiPlanner->CountLiveSpawnedFrom((gsDOM_SPAWN *)this) : 0;
        if (live >= (int)desc[48].pBrand)
            this->isBunchInProgress = 0;
    }
}
