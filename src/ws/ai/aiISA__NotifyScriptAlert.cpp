#include "../../headers/ws/ai/aiISA.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// aiISA::NotifyScriptAlert() @ 0x832A13C8
// ?NotifyScriptAlert@aiISA@@UAAXXZ (virtual)
// Raise an immediate team FROM_SCRIPT alert at the brain's current enemy nav position.
void aiISA::NotifyScriptAlert()
{
    aiSTATUS *st = this->pBrain->st.pointee;

    aiISA::ALERT_PARAMS params = {
        AI_ALERT_FROM_SCRIPT,
        st->enemy.posNav,
        dsTSTRING<char>("SCRIPT"),
        true,  // isAlertTeam
        false, // isUseDelay
    };
    this->Alert(params);
}
