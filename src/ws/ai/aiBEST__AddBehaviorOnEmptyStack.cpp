#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dmg/dmgSYSTEM.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"
#include "../../headers/ws/bhp/bhpSNS_PAR.h"
#include "../../headers/ws/bhp/bhpSTAY_PAR.h"
#include "../../headers/ws/bhp/bhpDEAD_PAR.h"

// aiBEST::AddBehaviorOnEmptyStack @ 0x83298740
// ?AddBehaviorOnEmptyStack@aiBEST@@EAAXXZ  (virtual)
//
// Choose and install a default behaviour when the stack would otherwise be empty:
//   - if the brain is in the "dying" death sub-state (AI_DIEST bit1) -> a "stay" behaviour;
//   - else if the brain is dead -> a "dead" behaviour (carrying a 1.0 default-damage event);
//   - else, first give the brain's own "SetBHVOnEmptyStack" hook a chance (when it has a nav system);
//     if that hook installs something (stack no longer empty) nothing more is done, otherwise fall
//     back to a "stay & notify sound" behaviour that executes immediately.
// The selected params are handed to SetBehaviorByParams. All param blocks live on the stack and are
// torn down on return.
void aiBEST::AddBehaviorOnEmptyStack()
{
    const dmgTYPE *dmgType = dmgSystem->GetDmgType(1);
    dmgDAMAGE   dmg(dmgType, 1.0f);
    bhpSNS_PAR  snsPar;
    bhpSTAY_PAR stayPar;
    bhpDEAD_PAR deadPar;

    deadPar.dmg = &dmg;
    snsPar.timeToExecute = 0.0f;

    bhpPARAMS *chosen;
    if ((pBrain->st.pointee->die.val & 2) != 0) {          // AI_DIEST bit1 — dying
        chosen = &stayPar;
    } else if (pBrain->IsDead()) {
        chosen = &deadPar;
    } else {
        if (pBrain->HaveNavSystem() && pBrain->SetBHVOnEmptyStack() && !IsEmpty())
            return;                                         // brain's own hook filled the stack
        chosen = reinterpret_cast<bhpPARAMS *>(&snsPar);    // bhpSNS_PAR's leading bytes are a bhpPARAMS
    }

    SetBehaviorByParams(chosen, nullptr);
}
