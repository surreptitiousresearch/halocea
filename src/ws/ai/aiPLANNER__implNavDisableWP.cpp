// aiPLANNER::implNavDisableWP @ 0x83247950
// ?implNavDisableWP@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@@Z
//
// SSL-exposed op: mark the named waypoint `wpName` disabled (nav flag bit0). Returns true on success;
// logs a "not found" diagnostic and returns false when the waypoint is absent.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern void _apLog(const char *fmt, ...);    // ?_apLog@@YAXPBDZZ — engine logger  boundary

bool aiPLANNER::implNavDisableWP(const dsTSTRING<char> &wpName)
{
    navWP *wp = this->navManager->FindWP(wpName.pBuffer->str);
    if (!wp) {
        _apLog("~AI,Error~NavDisableWP(): WP '%s' not found", wpName.pBuffer->str);
        return false;
    }
    wp->flags.state |= 1u;
    return true;
}
