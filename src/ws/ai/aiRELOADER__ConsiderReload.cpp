#include "../../headers/ws/ai/aiRELOADER.h"
#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_ssl_boundary.h" // aiBRAIN_DESC_UseFullReload
#include "../../headers/ws/dsc/dscDESC.h"

// aiRELOADER::ConsiderReload() @ 0x832B37B0
// ?ConsiderReload@aiRELOADER@@QAAXPAVpropWPN_AI_USABLE@@H@Z
//
// When `ctrl` exists and its clip has dropped to/below `perc_ammo_left`, reload it: a full
// (ammo-consuming + OnReload-notifying) reload if the brain's descriptor sets the "use full reload"
// flag, otherwise an instant reload.
void aiRELOADER::ConsiderReload(propWPN_AI_USABLE *ctrl, int perc_ammo_left)
{
    bool needReload = ctrl && !IsLoaded(ctrl, perc_ammo_left);
    if (needReload)
    {
        aiWEAPON *pWpn = this->pWpn;
        if (aiBRAIN_DESC_UseFullReload(pBrain->spDesc.pointee))
            pWpn->Reload(ctrl);
        else
            pWpn->ReloadInstantly(ctrl);
    }
}
