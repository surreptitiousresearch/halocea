// aiPLANNER::implNavSysMarkStatic @ 0x83247AE8
// ?implNavSysMarkStatic@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@_N@Z
//
// SSL-exposed op: set (or clear) the "static" flag on the nav-system named `nsName`. Returns true on
// success; logs "not found" and returns false when the nav-system is absent.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern void _apLog(const char *fmt, ...);    // ?_apLog@@YAXPBDZZ — engine logger  boundary

bool aiPLANNER::implNavSysMarkStatic(const dsTSTRING<char> &nsName, bool isStatic)
{
    navSYS *navSys = this->navManager->FindNavSys(nsName.pBuffer->str);
    if (!navSys) {
        _apLog("~AI,Ssl,Error~NavSysMarkStatic(): Nav System '%s' not found", nsName.pBuffer->str);
        return false;
    }
    navSys->isStatic = isStatic;
    return true;
}
