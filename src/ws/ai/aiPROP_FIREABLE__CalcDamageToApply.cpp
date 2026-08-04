#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"

// aiPROP_FIREABLE::CalcDamageToApply() @ 0x832A3DA8
// ?CalcDamageToApply@aiPROP_FIREABLE@@IBAMPBVdmgDAMAGE@@@Z  (protected const)
//
// Resolve the effective damage for an incoming hit:
//   * In revitalize mode (and not skipping it), a target that cannot bleed takes only token damage.
//   * A hit sourced by another AI brain is scaled by that brain's SSL distance-falloff curve.
//   * A player-sourced hit is routed through the friendly-fire rule.
//   * Otherwise the raw amount is used.
float aiPROP_FIREABLE::CalcDamageToApply(const dmgDAMAGE *pDmg) const
{
    bool bleedGate = this->IsRevitalizeMode() && !this->isSkipRevitalize;
    if (bleedGate && !aifireable_CanBleed(this->pOwner))
        return pDmg->amount * 9.9999997e-10f;

    aiBRAIN_IFACE *shooterBrain = aifireable_ShooterBrain(pDmg);
    if (shooterBrain && aifireable_IsBrainDesc(shooterBrain))
        return pDmg->amount * aifireable_AiDamageScale(shooterBrain, pDmg);
    if (aifireable_IsPlayer(shooterBrain))
        return this->CalcDamageFromPlayer(pDmg);
    return pDmg->amount;
}
