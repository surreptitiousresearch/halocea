#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/apCL.h"

// haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::GetEventMap @ 0x823C4B58 — the shared
// (id -> handler) sorted table for every event this class subscribes to. Function-local-static
// Meyers singleton (guard + atexit-registered destructor, per the usual C++ static-local-with-
// nontrivial-dtor lowering; not reproduced as separate guard/atexit statements here — `static`
// already gives the same one-time-construct/atexit-destroy semantics).
dsVECTOR<dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)>, 8> *
haloENGINE_CONTROL::EVENT_DISP_haloENGINE_CONTROL::GetEventMap()
{
    static dsVECTOR<dsPAIR<int, void (haloENGINE_CONTROL::*)(unsigned long, const dsPARAM_LIST &)>, 8> map(
        apCL{"d:\\projects\\code\\hcex\\sources\\halo_engine_ctrl.h", 0x40});
    return &map;
}
