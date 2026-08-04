#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::InitEventMap @ 0x823CC4E8 — one-time
// (function-local-static guarded) registration of every event this class handles: currently just
// "gsUserConfigChanged" -> OnConfigChanged.
void haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::InitEventMap()
{
    static bool initialized = false;
    if (initialized)
        return;

    int eventId = gEventMgr->RegisterEvent(dsSTRID("gsUserConfigChanged", false));
    // OnConfigChanged is declared with an `unsigned int` first param; RegisterEventEntry wants
    // `unsigned long` (same 32-bit width, distinct MSVC type). Cast the member-fn pointer to match.
    RegisterEventEntry(eventId,
        (void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)) &haloENGINE_CONTROL::OnConfigChanged);
    initialized = true;
}
