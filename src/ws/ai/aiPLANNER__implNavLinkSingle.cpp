// aiPLANNER::implNavLinkSingle @ 0x83247798
// ?implNavLinkSingle@aiPLANNER@@UAAXABV?$dsTSTRING@D@@000@Z
//
// SSL-exposed op: create a single (non-mutual) scripted navigation link from waypoint `wpA` to `wpB`
// inside the nav-system named `sysName`, tagged with the smart-object name `smtObjName`. Mirrors
// implNavLink but passes autolink=false (the 5th LinkScriptedOp arg) so only the one directed edge
// is created. When the planner has no nav manager or the named nav-system does not exist, log a
// "not found" diagnostic instead.
//
// NOTE: the decompiler packs the four dsTSTRING& args into __int64 register pairs and dereferences
// `pBuffer + 12` (the buffer's char data) manually; reconstructed via CStr()/pBuffer->str.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern aiPLANNER *aiPlanner;                 // ai08 planner singleton (aiEXIFACE.h)
extern void _apLog(const char *fmt, ...);    // ?_apLog@@YAXPBDZZ — engine logger  boundary

void aiPLANNER::implNavLinkSingle(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                                  const dsTSTRING<char> &wpB, const dsTSTRING<char> &smtObjName)
{
    if (aiPlanner && aiPlanner->GetNavManager()) {
        navSYS *navSys = aiPlanner->GetNavManager()->FindNavSys(sysName.pBuffer->str);
        if (navSys) {
            navSys->LinkScriptedOp(wpA.pBuffer->str, wpB.pBuffer->str, true, false, smtObjName);
            return;
        }
    }
    _apLog("~AI~Manager: '%s' nav-system is not found", sysName.pBuffer->str);
}
