#include "../../headers/ws/ai/aiISA.h"
#include "../../headers/ws/ai/aiSENSES_setsen_boundary.h" // enableAllAlerts
#include "../../headers/ws/ds/dsTSTRING.h"
#include <string.h>

// aiISA::SetPar() @ 0x832A0030
// ?SetPar@aiISA@@UAA_NABV?$dsTSTRING@D@@M@Z
//
// Virtual: apply a per-alert enable/disable tunable. Each "enableAlert_*" key toggles one alert
// kind; "enableAllAlerts" toggles them all; anything else falls through to the interface base.
bool aiISA::SetPar(const dsTSTRING<char> &key, float val)
{
    const char *k = key.CStr();
    const bool on = val > 0.5f;

    if (strcmp(k, "enableAlert_SEE") == 0)      { EnableAlert(AI_ALERT_SEE, on);      return true; }
    if (strcmp(k, "enableAlert_HEAR") == 0)     { EnableAlert(AI_ALERT_HEAR, on);     return true; }
    if (strcmp(k, "enableAlert_DAMAGE") == 0)   { EnableAlert(AI_ALERT_DAMAGE, on);   return true; }
    if (strcmp(k, "enableAlert_PHYS_OBJ") == 0) { EnableAlert(AI_ALERT_PHYS_OBJ, on); return true; }
    if (strcmp(k, "enableAlert_INTEROP") == 0)  { EnableAlert(AI_ALERT_INTEROP, on);  return true; }

    if (key == enableAllAlerts)
    {
        EnableAlert(AI_ALERT_SEE, on);
        EnableAlert(AI_ALERT_HEAR, on);
        EnableAlert(AI_ALERT_DAMAGE, on);
        EnableAlert(AI_ALERT_PHYS_OBJ, on);
        EnableAlert(AI_ALERT_INTEROP, on);
        return true;
    }

    return aiISA_IFACE::SetPar(key, val);
}
