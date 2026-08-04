#include "../../headers/ws/ai/aiISA.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h" // _apLog

// aiISA::EnableAlert() @ 0x832A04E8
// ?EnableAlert@aiISA@@UAAXW4AI_ALERT@@_N@Z
//
// Virtual: enable/disable one report-alert bit and log the change. ALERT_FROM_SCRIPT is always
// enabled and cannot be toggled; unknown alert kinds log an error.
void aiISA::EnableAlert(AI_ALERT alert, bool enable)
{
    const char *what = enable ? "enabled" : "disabled";
    const char *name = pBrain->names.nameInst;

    switch (alert)
    {
    case AI_ALERT_SEE:
        if (enable) state.val |= AI_ISAST_REPORT_SEE; else state.val &= ~AI_ISAST_REPORT_SEE;
        _apLog("~AI,Senses~'%s': alert \"SEE\" is %s.", name, what);
        break;
    case AI_ALERT_HEAR:
        if (enable) state.val |= AI_ISAST_REPORT_HEAR; else state.val &= ~AI_ISAST_REPORT_HEAR;
        _apLog("~AI,Senses~'%s': alert \"HEAR\" is %s.", name, what);
        break;
    case AI_ALERT_DAMAGE:
        if (enable) state.val |= AI_ISAST_REPORT_DAMAGE; else state.val &= ~AI_ISAST_REPORT_DAMAGE;
        _apLog("~AI,Senses~'%s': alert \"DAMAGE\" is %s.", name, what);
        break;
    case AI_ALERT_PHYS_OBJ:
        if (enable) state.val |= AI_ISAST_REPORT_PHYS_OBJ; else state.val &= ~AI_ISAST_REPORT_PHYS_OBJ;
        _apLog("~AI,Senses~'%s': alert \"PHYS_OBJ\" is %s.", name, what);
        break;
    case AI_ALERT_INTEROP:
        if (enable) state.val |= AI_ISAST_REPORT_INTEROP; else state.val &= ~AI_ISAST_REPORT_INTEROP;
        _apLog("~AI,Senses~'%s': alert \"INTEROP\" is %s.", name, what);
        break;
    case AI_ALERT_FROM_SCRIPT:
        _apLog("~AI,Senses,Error~'%s': cannot enable/disable ALERT_FROM_SCRIPT. It is always enabled.", name);
        break;
    default:
        _apLog("~AI,Senses,Error~'%s': enable/disable alert: unknown alert", name);
        break;
    }
}
