#include "../../headers/ws/ai/aiRELOADER.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"

// aiRELOADER::IsNeedToReload() @ 0x832B36F8
// ?IsNeedToReload@aiRELOADER@@QBA_NPBVpropWPN_AI_USABLE@@H@Z
//
// A reload is needed when the controller exists and its clip is not loaded above `perc_ammo_left`.
bool aiRELOADER::IsNeedToReload(const propWPN_AI_USABLE *ctrl, int perc_ammo_left) const
{
    return ctrl && !IsLoaded(ctrl, perc_ammo_left);
}
