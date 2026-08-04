#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::SetProduct @ 0x823CC5D8 — bind this dispatcher
// to its owning object: sets pObj, then (only when non-null) runs InitEventMap() once and
// SubscribeEvent()s every entry in GetEventMap() against the global dsEVENT_MGR.
void haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::SetProduct(haloENGINE_CONTROL *_pObj)
{
    pObj = _pObj;
    if (!_pObj)
        return;

    InitEventMap();

    auto *eventMap = GetEventMap();
    for (int i = 0; i < eventMap->Length(); ++i)
        gEventMgr->SubscribeEvent((*GetEventMap())[i].key, this, 0);
}
