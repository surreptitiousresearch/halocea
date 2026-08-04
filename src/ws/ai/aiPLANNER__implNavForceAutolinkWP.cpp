// aiPLANNER::implNavForceAutolinkWP @ 0x83247A50
// ?implNavForceAutolinkWP@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z
//
// SSL-exposed op: re-enable the named waypoint `wpName`, force it to autolink to its neighbours, and
// queue a high-obstacle jump batch build for it. Returns true on success; logs "not found" and
// returns false when the waypoint is absent.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern void _apLog(const char *fmt, ...);    // ?_apLog@@YAXPBDZZ — engine logger  boundary

bool aiPLANNER::implNavForceAutolinkWP(const dsTSTRING<char> &wpName)
{
    short wpID = this->navManager->FindWP_ID(wpName.pBuffer->str);
    navWP *wp = this->navManager->GetWP(wpID);
    if (!wp) {
        _apLog("~AI,Error~NavForceAutolinkWP(): WP '%s' not found", wpName.pBuffer->str);
        return false;
    }
    wp->flags.state &= ~1u;
    this->navManager->AutolinkWP(wpID);
    this->navManager->AddHOJumpBatch(wp, wpID);
    return true;
}
