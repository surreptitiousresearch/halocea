#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"

// aiDOM_SPAWN::Update @ 0x83264AF0  ?Update@aiDOM_SPAWN@@MAAXXZ (virtual)
// Per-frame tick: advance the spawn/kill timers and, while the domain is active, fire a spawn
// wave whenever the readiness gate opens. Gated on the desc having a class list (desc[24]) and on
// having constructed spawn points.
void aiDOM_SPAWN::Update()
{
    if (!this->spDesc.pointee[24].__vftable)
        return;
    if (!this->pointsList.nElem)
        return;

    float killTime = this->timeAfterKillCur;
    unsigned int active = this->stateDom.state & 1;
    this->timeAfterSpawn = this->timeAfterSpawn + gsElapsedTime;
    this->timeAfterKillCur = killTime + gsElapsedTime;

    if (active)
    {
        if (this->IsReadySpawn())
            this->DoSpawn();
    }
}
