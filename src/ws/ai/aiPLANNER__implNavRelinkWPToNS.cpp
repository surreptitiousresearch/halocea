#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// aiPLANNER::implNavRelinkWPToNS(const dsTSTRING<char>&, const dsTSTRING<char>&) @ 0x83247DB8
// ?implNavRelinkWPToNS@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@0@Z  (virtual)
// SSL op: move waypoint `wpName` into the nav-system named `nsName`. Returns false (and logs) when
// the nav-system or waypoint cannot be found, or the relink itself fails (with the reason string).
bool aiPLANNER::implNavRelinkWPToNS(const dsTSTRING<char> &wpName, const dsTSTRING<char> &nsName)
{
    navSYS *ns = navManager->FindNavSys(nsName.pBuffer->str);
    if (!ns)
    {
        _apLog("~AI,Ssl,Error~NavRelinkWPToNS(): Nav System '%s' not found", nsName.pBuffer->str);
        return false;
    }

    short wpID = navManager->FindWP_ID(wpName.pBuffer->str);
    navWP *wp = navManager->GetWP(wpID);
    if (!wp)
    {
        _apLog("~AI,Error~NavRelinkWPToNS(): WP '%s' not found", wpName.pBuffer->str);
        return false;
    }

    dsTSTRING<char> reason(""); // relink failure reason (filled by RelinkWPToNS)
    if (navManager->RelinkWPToNS(ns, wpID, wp, reason))
        return true;

    _apLog("~AI,Ssl,Error~NavRelinkWPToNS(%s, %s) fails: %s",
           wpName.pBuffer->str, nsName.pBuffer->str, reason.pBuffer->str);
    return false;
}
