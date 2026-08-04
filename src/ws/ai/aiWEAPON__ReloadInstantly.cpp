// aiWEAPON::ReloadInstantly @ 0x832B2D40
// ?ReloadInstantly@aiWEAPON@@UAAXPAVpropWPN_AI_USABLE@@@Z  (virtual)
//
// Instantly reload the weapon driven by controller `ctrl`: fetch its equipped weapon, and — when the
// weapon's shooter still holds ammo state to reset — send a cmdWPN_RELOAD FSM command for every
// attack the shooter descriptor defines. A null controller / weapon, or an already-empty shooter,
// makes this a no-op.
#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiWEAPON_ReloadInstantly_boundary.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h" // wpnWEAPON (canonical) + SendCmd
#include "../../headers/ws/wpn/wpnSHOOTER.h"

void aiWEAPON::ReloadInstantly(propWPN_AI_USABLE *ctrl)
{
    if (!ctrl)
        return;

    wpnWEAPON *weapon = ctrl->weapon();
    if (!weapon)
        return;

    if (weapon->shooter.IsEmpty())
        return;

    for (int atkID = 0; atkID < weapon->shooter.pDesc->attacks.nElem; ++atkID)
    {
        fsmWPN_ATK_INFO atkInfo;
        atkInfo.atkID = atkID;
        atkInfo.params = nullptr;
        weapon->SendCmd(&cmdWPN_RELOAD, &atkInfo);
    }
}
