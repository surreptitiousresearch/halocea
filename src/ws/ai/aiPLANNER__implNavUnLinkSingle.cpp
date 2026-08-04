// aiPLANNER::implNavUnLinkSingle @ 0x832480E8
// ?implNavUnLinkSingle@aiPLANNER@@UAAXABV?$dsTSTRING@D@@00@Z
//
// SSL op: remove the single (one-directional) scripted navigation link from waypoint `wpA` to
// `wpB` inside the nav-system named `sysName`. Identical to implNavUnLink except LinkScriptedOp is
// invoked with single=false (only the wpA->wpB direction). Logs a diagnostic when the planner has
// no nav manager or the named nav-system does not exist.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern aiPLANNER *aiPlanner;                 // ai08 planner singleton (aiEXIFACE.h)
extern void _apLog(const char *fmt, ...);    // ap subsystem logger  boundary

void aiPLANNER::implNavUnLinkSingle(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                                    const dsTSTRING<char> &wpB)
{
    if (aiPlanner && aiPlanner->GetNavManager())
    {
        navSYS *navSys = aiPlanner->GetNavManager()->FindNavSys(sysName.CStr());
        if (navSys)
        {
            dsTSTRING<char> smtObjName("");
            navSys->LinkScriptedOp(wpA.CStr(), wpB.CStr(), false, false, smtObjName);
            return;
        }
    }
    _apLog("~AI,Error~Manager: '%s' nav-system is not found", sysName.CStr());
}
