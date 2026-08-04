// aiPLANNER::implNavUnLink @ 0x83247FE8
// ?implNavUnLink@aiPLANNER@@UAAXABV?$dsTSTRING@D@@00@Z
//
// SSL op: remove the scripted (bidirectional) navigation link between waypoints `wpA` and `wpB`
// inside the nav-system named `sysName`. LinkScriptedOp with make=false (remove) and single=true
// (both directions), tagged with an empty smart-object name. Logs a diagnostic when the planner
// has no nav manager or the named nav-system does not exist.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern aiPLANNER *aiPlanner;                 // ai08 planner singleton (aiEXIFACE.h)
extern void _apLog(const char *fmt, ...);    // ap subsystem logger  boundary

void aiPLANNER::implNavUnLink(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                              const dsTSTRING<char> &wpB)
{
    if (aiPlanner && aiPlanner->GetNavManager())
    {
        navSYS *navSys = aiPlanner->GetNavManager()->FindNavSys(sysName.CStr());
        if (navSys)
        {
            dsTSTRING<char> smtObjName("");
            navSys->LinkScriptedOp(wpA.CStr(), wpB.CStr(), false, true, smtObjName);
            return;
        }
    }
    _apLog("~AI,Error~Manager: '%s' nav-system is not found", sysName.CStr());
}
