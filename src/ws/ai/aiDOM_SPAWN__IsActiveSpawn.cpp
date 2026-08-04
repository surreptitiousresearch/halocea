#include "../../headers/ws/ai/aiDOM_SPAWN.h"

// aiDOM_SPAWN::IsActiveSpawn @ 0x83264800  ?IsActiveSpawn@aiDOM_SPAWN@@AAAHXZ
// Active when the domain is enabled (state bit0), not locked, and not "spawn-done" (state bit10);
// and, if a spawn cap (desc[47].__vftable slot) is set, the cap has not yet been reached.
int aiDOM_SPAWN::IsActiveSpawn()
{
    unsigned int state = this->stateDom.state;
    if ((state & 1) == 0 || this->nmbLocks > 0 || ((state >> 10) & 1) != 0)
        return 0;

    int cap = (int)this->spDesc.pointee[47].__vftable;
    if (cap <= 0)
        return 1;
    if (this->nSpawned < cap)
        return 1;
    return 0;
}
