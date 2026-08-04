// aiPLANNER::implNavSysPenalizeByDomain @ 0x83247B58
// ?implNavSysPenalizeByDomain@aiPLANNER@@UAA_NABV?$dsTSTRING@D@@0H@Z
//
// SSL-exposed op: raise the traversal penalty of every waypoint in domain `nameDom` of nav-system
// `nameNS` by `level` (clamped to 1..10). Returns true on success; logs and returns false when the
// nav-system or the domain WP-set is absent.
//
// NOTE: the original source calls navSYS::PenalizeWPSet, which the compiler inlined to
// navGPH::PenalizeWPSet on the navSYS's embedded gph sub-object (navSYS+0x08). Modeled here via the
// navSYS::PenalizeWPSet forwarder to keep the navGPH interior opaque.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern void _apLog(const char *fmt, ...);    // ?_apLog@@YAXPBDZZ — engine logger  boundary

bool aiPLANNER::implNavSysPenalizeByDomain(const dsTSTRING<char> &nameNS,
                                           const dsTSTRING<char> &nameDom, int level)
{
    navSYS *navSys = this->navManager->FindNavSys(nameNS.pBuffer->str);
    if (!navSys) {
        _apLog("~AI,Error~NavSysPenalizeByDomain(): Nav-System '%s' not found", nameNS.pBuffer->str);
        return false;
    }

    navWP_SET *wpSet = navSys->GetWpSet(nameDom);
    if (!wpSet) {
        _apLog("~AI,Ssl,Error~NavSysPenalizeByDomain(): '%s' can't get set of WPs",
               nameDom.pBuffer->str);
        return false;
    }

    if (level < 1) {
        level = 1;
        _apLog("~AI,Ssl~NavSysPenalizeByDomain(): level is corrected to 1");
    } else if (level > 10) {
        level = 10;
        _apLog("~AI,Ssl~NavSysPenalizeByDomain(): level is corrected to 10");
    }

    navSys->PenalizeWPSet(wpSet, level);
    return true;
}
