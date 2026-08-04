#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiSTRIKE.h"
#include "../../headers/ws/ai/aiS_VARS.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::IsAimed @ 0x83295A78
// ?IsAimed@aiWEAPON@@IBA_NABUm3dV@@ABUaiSTRIKE@@@Z
//
// Is the weapon considered aimed at `posTarget` for this concrete strike? Blind-shoot / suppressed
// weapon states short-circuit to "aimed"; otherwise the strike descriptor's aim-check mode selects
// a pos/dir alignment test, a body melee test, or unconditional-aimed.
bool aiWEAPON::IsAimed(const m3dV &posTarget, const aiSTRIKE &strike) const
{
    GetCtrl(strike.vars->tie); // resolve/validate the controller (result unused by the aim test)

    const unsigned int wpnState = pBrain->st.pointee->wpn.val;
    if (((wpnState >> 13) & 1) || ((wpnState >> 12) & 1) || isOKBlindShoot)
        return true;

    int aimCheck = strike.desc->typeIsAimed;
    if (aimCheck == 4) // aimUNDEF — resolve from the controller
        aimCheck = static_cast<int>(GetAimCheck(strike.vars->tie.idxCtrl));

    if ((unsigned int)aimCheck <= 3) {
        switch (aimCheck) {
        case 0: // aimBY_WPN
        case 1: // aimBY_BODY
            return IsAimedByPosAndDir(posTarget, strike);
        case 2: // aimBY_FSM — melee alignment via the body
            return pBrain->body.pointee->IsAimedForMelee();
        }
        return true; // case 3 — aimALWAYS_AIMED
    }
    return false;
}
