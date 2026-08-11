#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::OnEvent @ 0x823C989C — look up `event->id` in
// GetEventMap() (binary search) and invoke the bound haloENGINE_CONTROL member function on pObj
// with (event->userId, event->paramList). A no-op while pObj is null (before SetProduct).
void haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::OnEvent(const dsEVENT *event)
{
    if (!pObj)
        return;

    auto *eventMap = GetEventMap();
    int idx = eventMap->FindSorted<int>(event->id);
    if (!IGNORE_STRONG_ASSERT && idx < 0)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash("idx >= 0", "d:\\projects\\code\\hcex\\sources\\halo_engine_ctrl.h", 64, "");

    auto &entry = (*GetEventMap())[idx];
    (pObj->*entry.val)(event->userId, event->paramList);
}
