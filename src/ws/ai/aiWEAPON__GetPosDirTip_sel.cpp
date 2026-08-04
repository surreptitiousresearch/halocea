#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiS_TIE.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiWEAPON::GetPosDirTip @ 0x83295B50
// ?GetPosDirTip@aiWEAPON@@UBA_NAAUm3dV@@0@Z  (virtual)
//
// Muzzle tip pos/dir for the module's "current" strike: prefer the selected tie; otherwise scan the
// registered controllers for the first active, custom-weapon-enabled one and query its tip.
bool aiWEAPON::GetPosDirTip(m3dV &pos, m3dV &dir) const
{
    if (!tieSelected.IsNull())
        return GetPosDirTip(tieSelected, pos, dir);

    const int nElem = ctrlsList.nElem;
    for (int i = 0; i < nElem; ++i) {
        const CTRL_INFO &info = ctrlsList[i];
        // Scoped weak reference keeps the controller alive across the query (matches the binary's
        // per-iteration handle copy + release).
        ds::WEAK_PTR<propWPN_AI_USABLE> ctrlRef(info.ctrl);
        propWPN_AI_USABLE *ctrl = ctrlRef.GetPtr();
        if (ctrl && info.isActive && info.isEnabledByCustomWeapon)
            return GetPosDirTip(ctrl, pos, dir);
    }
    return false;
}
