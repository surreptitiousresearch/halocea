#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiWEAPON::SetupTarget @ 0x83296600
// ?SetupTarget@aiWEAPON@@IAAXXZ
//
// Recompute the weapon target (unless the module is in the "absent" no-weapon state) and mirror it
// into the brain's debug "passed weapon target" for visualisation.
void aiWEAPON::SetupTarget()
{
    if (IsAbsent())
        return;

    RecalcTarget();

    pBrain->dbg.posPassedWpnTgt.x = pBrain->cmd.posWpnTgt.x;
    pBrain->dbg.posPassedWpnTgt.y = pBrain->cmd.posWpnTgt.y;
    pBrain->dbg.posPassedWpnTgt.z = pBrain->cmd.posWpnTgt.z;
}
