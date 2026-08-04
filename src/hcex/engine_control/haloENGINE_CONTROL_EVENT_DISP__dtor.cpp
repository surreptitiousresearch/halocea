#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::~EVENT_DISP_haloENGINE_CONTROL @ 0x823C97AC —
// unsubscribe from every event in GetEventMap(), then restore the dsEVENT_HANDLER base vtable
// pointer (the compiler's usual base-class-destructor-order teardown; the decompile shows this as
// an explicit final vtable store rather than an implicit base dtor call, reproduced as observed —
// the base class destructor runs automatically at the closing brace regardless).
haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::~EVENT_DISP_haloENGINE_CONTROL()
{
    auto *eventMap = GetEventMap();
    for (int i = 0; i < eventMap->Length(); ++i)
        gEventMgr->UnsubscribeEvent((*eventMap)[i].key, this, 0);
}
