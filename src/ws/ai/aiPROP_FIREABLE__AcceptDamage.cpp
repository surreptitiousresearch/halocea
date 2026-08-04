#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"

// aiPROP_FIREABLE::AcceptDamage() @ 0x832A4F58
// ?AcceptDamage@aiPROP_FIREABLE@@MAAXPBVdmgDAMAGE@@@Z  (protected virtual)
//
// Entry point for an incoming hit. Applies the debug damage multiplier, drops ignored hits, then:
// if already dead/dying the shot passes through; otherwise the owner pre-damage hook + base accept
// run, a shooting-precision-stat record is taken, and (if still alive) the mind is notified of the
// wound. Finally, when the owner has no hit/death FSM, the body wound event is raised.
void aiPROP_FIREABLE::AcceptDamage(const dmgDAMAGE *pDmg)
{
    // STRONG_ASSERT: gsIs<aiBRAIN>(Owner(this))  (debug-only guard)  boundary
    if (!aifireable_OwnerIsBrain(this->pOwner))
    { /* strong-assert crash path (boundary) */ }

    dmgDAMAGE *dmg = const_cast<dmgDAMAGE *>(pDmg); // decompiler writes through the const param
    dmg->amount = dmg->amount * aifireable_DbgDmgScale();

    if (this->IsIgnoring(dmg))
        return;

    unsigned int state = this->state.state;
    iaIACTOR *pOwner = this->pOwner;
    if (((state >> 1) & 1) != 0 || ((state >> 2) & 1) != 0)
    {
        this->propFIREABLE::ShootThrough(pDmg);
    }
    else
    {
        aifireable_OwnerPreDamage(pOwner, pDmg);
        this->propFIREABLE::AcceptDamage(pDmg);

        // record a shooting-precision hit (internally guarded on a non-empty typeGun + active stat)
        aifireable_RecordShootingPrecision(this, pDmg);

        unsigned int state2 = this->state.state;
        if (((state2 >> 1) & 1) == 0 && ((state2 >> 2) & 1) == 0)
            aifireable_NoticeWounded(pOwner, pDmg);
    }

    // when there's no hit/death FSM (and the owner status wound bit is clear — folded into helper),
    // raise the body "wounding" event.
    if (!this->propFIREABLE::HasHitDeathSys())
        aifireable_SendBodyHitEvent(pOwner, dmg, /*wounding=*/true);
}
