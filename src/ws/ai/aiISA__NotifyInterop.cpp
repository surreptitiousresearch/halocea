#include "../../headers/ws/ai/aiISA.h"

// aiISA::NotifyInterop(const m3dV&) @ 0x832A11F0
// ?NotifyInterop@aiISA@@UAAXABUm3dV@@@Z (virtual)
// When interop reports are enabled, raise a delayed (non-team) INTEROP alert at `posAlert`.
void aiISA::NotifyInterop(const m3dV &posAlert)
{
    if (this->state.val & AI_ISAST_REPORT_INTEROP)
    {
        aiISA::ALERT_PARAMS params = {
            AI_ALERT_INTEROP,
            posAlert,
            dsTSTRING<char>("INTEROP"),
            false, // isAlertTeam
            true,  // isUseDelay
        };
        this->Alert(params);
    }
}
