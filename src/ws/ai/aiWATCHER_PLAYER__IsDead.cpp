#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"
#include "../../headers/ws/prop/propFIREABLE.h"

// aiWATCHER_PLAYER::IsDead @ 0x8328EB58
// ?IsDead@aiWATCHER_PLAYER@@UBA_NXZ
// Virtual const: true when the tracked player's fireable/health component reports a dead or
// dying state. The state word (propFIREABLE::state @0x30) carries the death bits at positions 1
// (dead) and 2 (dying); either set => dead. A missing fireable component reports not-dead.
bool aiWATCHER_PLAYER::IsDead() const
{
    propFIREABLE *fireable = pFireable;
    if (!fireable)
        return false;
    unsigned int state = fireable->state.state;
    return ((state >> 1) & 1) != 0 || ((state >> 2) & 1) != 0;
}
