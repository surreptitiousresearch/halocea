// aiPLANNER::implNavLink @ 0x832476E0
// ?implNavLink@aiPLANNER@@UAAXABV?$dsTSTRING@D@@000@Z
//
// SSL-exposed op: create a scripted navigation link between waypoints `wpA` and `wpB` inside the
// nav-system named `sysName`, tagged with the smart-object name `smtObjName`. When the planner has
// no nav manager or the named nav-system does not exist, log a "not found" diagnostic instead.
//
// NOTE: the decompiler packs the four dsTSTRING& args into __int64 register pairs and dereferences
// `pBuffer + 12` (the buffer's char data) manually; reconstructed via CStr() / the by-ref args.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern aiPLANNER *aiPlanner;                 // ai08 planner singleton (aiEXIFACE.h)
extern void _apLog(const char *fmt, ...);    // ?_apLog@@YAXPBDZZ — engine logger  boundary

void aiPLANNER::implNavLink(const dsTSTRING<char> &sysName, const dsTSTRING<char> &wpA,
                            const dsTSTRING<char> &wpB, const dsTSTRING<char> &smtObjName)
{
    if (aiPlanner && aiPlanner->GetNavManager()) {
        navSYS *navSys = aiPlanner->GetNavManager()->FindNavSys(sysName.CStr());
        if (navSys) {
            navSys->LinkScriptedOp(wpA.CStr(), wpB.CStr(), true, true, smtObjName);
            return;
        }
    }
    _apLog("~AI~Manager: '%s' nav-system is not found", sysName.CStr());
}
