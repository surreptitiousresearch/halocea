#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// aiPLANNER::implNavSysRemove(const dsTSTRING<char>&) @ 0x83247CD0
// ?implNavSysRemove@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z  (virtual)
// SSL op: tear down the nav-system named `nsName`. Every active AI unit currently localized to it
// is re-localized (moved off it). Returns false (and logs) when no such nav-system exists.
bool aiPLANNER::implNavSysRemove(const dsTSTRING<char> &nsName)
{
    navSYS *ns = navManager->FindNavSys(nsName.pBuffer->str);
    if (!ns)
    {
        _apLog("~AI,Ssl,Error~NavSysRemove(): Nav System '%s' not found", nsName.pBuffer->str);
        return false;
    }

    ns->Term();

    for (int i = 0; i < wah.arrUnits.nElem; ++i)
    {
        aiWATCHER_BRAIN *watcher = wah.PeekActive(i);
        if (watcher)
        {
            aiBRAIN *br = watcher->brain.GetPtr();
            if (br && br->GetNavSystem() == ns)
                LocalizeToNavSystem(br);
        }
    }
    return true;
}
