#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"

// aiPROP_FIREABLE::CalcDamageFromPlayer() @ 0x832A3C30
// ?CalcDamageFromPlayer@aiPROP_FIREABLE@@IBAMPBVdmgDAMAGE@@@Z  (protected const)
//
// Player-sourced friendly fire: unless the AI-friendly-fire debug switch is on or the owner is
// immune, scale the amount down to a token value (x 1e-9) so it registers without hurting.
float aiPROP_FIREABLE::CalcDamageFromPlayer(const dmgDAMAGE *pDmg) const
{
    if (!aifireable_FriendlyFireAI()
        && this->pOwner
        && aifireable_OwnerImmuneToPlayerFF(this->pOwner))
    {
        return pDmg->amount * 9.9999997e-10f;
    }
    return pDmg->amount;
}
