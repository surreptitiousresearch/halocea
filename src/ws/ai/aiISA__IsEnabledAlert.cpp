#include "../../headers/ws/ai/aiISA.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // _apLog

// aiISA::IsEnabledAlert() @ 0x832A03B8
// ?IsEnabledAlert@aiISA@@UBA_NW4AI_ALERT@@@Z
//
// Virtual const: report whether `alert` is currently enabled. ALERT_FROM_SCRIPT is always enabled;
// unknown alert kinds log an error and report disabled.
bool aiISA::IsEnabledAlert(AI_ALERT alert) const
{
    switch (alert)
    {
    case AI_ALERT_SEE:         return (state.val & AI_ISAST_REPORT_SEE) != 0;
    case AI_ALERT_HEAR:        return (state.val & AI_ISAST_REPORT_HEAR) != 0;
    case AI_ALERT_DAMAGE:      return (state.val & AI_ISAST_REPORT_DAMAGE) != 0;
    case AI_ALERT_PHYS_OBJ:    return (state.val & AI_ISAST_REPORT_PHYS_OBJ) != 0;
    case AI_ALERT_INTEROP:     return (state.val & AI_ISAST_REPORT_INTEROP) != 0;
    case AI_ALERT_FROM_SCRIPT: return true;
    default:
        _apLog("~AI,Senses,Error~IsAlertEnabled() alert: unknown alert");
        return false;
    }
}
