#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"

// aiPROP_FIREABLE::Die() @ 0x832A4AC0
// ?Die@aiPROP_FIREABLE@@MAAXPBVdmgDAMAGE@@@Z  (protected virtual)
//
// Death handler for a resolved lethal hit. Guards against re-entry once dead/dying, then either
// bleeds (revitalize mode) or runs the full death path: OnDeath/OnAIKill SSL events, kill
// notification, tower-man topple, and the body "die" FSM event. The heavy branch bodies (SSL event
// dispatch over ssl/dsDATA/propContainer) lie past this wave's bounded descent — see boundaries.
void aiPROP_FIREABLE::Die(const dmgDAMAGE *pDmg)
{
    unsigned int state = this->state.state;
    if (((state >> 1) & 1) != 0 || ((state >> 2) & 1) != 0)
        return; // already dead/dying

    this->state.state |= 2u; // mark dead

    bool bleedGate = this->IsRevitalizeMode() && !this->isSkipRevitalize;
    if (bleedGate)
        aifireable_HandleBleed(this);
    else
        aifireable_HandleDeath(this, pDmg);
}
