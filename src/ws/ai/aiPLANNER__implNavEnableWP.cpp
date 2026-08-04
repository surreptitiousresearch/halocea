// aiPLANNER::implNavEnableWP @ 0x832479D0
// ?implNavEnableWP@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z
//
// SSL-exposed op: clear the disabled flag (nav flag bit0) on the named waypoint `wpName`. Returns
// true on success; logs a "not found" diagnostic and returns false when the waypoint is absent.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern void _apLog(const char *fmt, ...);    // ?_apLog@@YAXPBDZZ — engine logger  boundary

bool aiPLANNER::implNavEnableWP(const dsTSTRING<char> &wpName)
{
    navWP *wp = this->navManager->FindWP(wpName.pBuffer->str);
    if (!wp) {
        _apLog("~AI,Error~NavEnableWP(): WP '%s' not found", wpName.pBuffer->str);
        return false;
    }
    wp->flags.state &= ~1u;
    return true;
}
