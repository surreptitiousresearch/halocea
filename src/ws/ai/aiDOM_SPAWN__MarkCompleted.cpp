#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"

// aiDOM_SPAWN::MarkCompleted @ 0x832641F8  ?MarkCompleted@aiDOM_SPAWN@@AAAXXZ
// Flag the spawn domain COMPLETED (once): set the "completed" bit in the domain state and the
// pending-SSL-event bit in the actor state, then log.
void aiDOM_SPAWN::MarkCompleted()
{
    unsigned int state = this->stateDom.state;
    if (((state >> 10) & 1) == 0)
    {
        this->stateDom.state = state | 0x400;
        this->stateSslEv.state |= 0x800u;
        _apLog("~Spawn~domSpawn '%s': COMPLETED", this->GetName().pBuffer->str);
    }
}
