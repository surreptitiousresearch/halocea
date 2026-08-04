#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"

// aiPROP_FIREABLE::SendDamage() @ 0x832A4A30
// ?SendDamage@aiPROP_FIREABLE@@UAAXPAVdmgDAMAGE@@@Z  (vtbl)
//
// Resolve the effective damage, broadcast it through the base fireable, then (when the owner runs a
// hit/death FSM) raise the body "apply hit" event.
void aiPROP_FIREABLE::SendDamage(dmgDAMAGE *pDmg)
{
    pDmg->amount = this->CalcDamageToApply(pDmg);
    this->propFIREABLE::SendDamage(pDmg);

    if (this->propFIREABLE::HasHitDeathSys())
        aifireable_SendBodyHitEvent(this->pOwner, pDmg, /*wounding=*/false);
}
