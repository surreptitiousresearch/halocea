#include "../../headers/ws/ai/aiISA.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPlanner
#include "../../headers/ws/ai/ai08_debug_gates_boundary.h" // _skip
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // _apLog

// aiISA::Alert() @ 0x8329FF50
// ?Alert@aiISA@@MAA_NABUALERT_PARAMS@1@@Z
//
// Protected virtual: raise an alert. When not already alerted (and not globally suppressed), record
// the alert position, queue the delayed OnAlert dispatch, latch the ALERTED state, and optionally
// propagate the alert to the whole team.
bool aiISA::Alert(const ALERT_PARAMS &params)
{
    if (!IsAlerted() && !_skip)
    {
        _apLog("~AI,Senses~'%s': alerted by %s", pBrain->names.nameInst, params.reason.CStr());

        pBrain->cmd.posAlert = params.pos;
        pBrain->mind.PostDelayedOnAlert(params.type, params.isUseDelay);
        state.val |= AI_ISAST_ALERTED;

        if (params.isAlertTeam && aiPlanner)
        {
            const propGAME_INFO_TEAM *team = pBrain->GetTeamID();
            aiPlanner->AlertTeam(*team, params.pos);
        }
    }
    return true;
}
