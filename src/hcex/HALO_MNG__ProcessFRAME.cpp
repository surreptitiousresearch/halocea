#include "../headers/hcex/HALO_MNG.h"
#include "../headers/hcex/HALO_MNG_boundary.h"

// 0x823E9F20 — per-frame tick. On a dbg-triggered level-reload request (dbg_forceReload.value),
// lazily registers the "OnLevelReloadRequest" event (function-local-static guard, matching the
// binary's `local static guard` bit) and signals it with the configured default map/checkpoint
// names as named parameters; then runs one Blam engine frame (hcex_process_frame) and updates
// HCEX-side animation/cinematics for the elapsed time.
void HALO_MNG::ProcessFRAME()
{
    if ( dbg_forceReload.value )
    {
        static int evid_OnLevelReloadRequest;
        static bool s_eventRegistered;

        if ( !s_eventRegistered )
        {
            s_eventRegistered = true;
            evid_OnLevelReloadRequest = gEventMgr->RegisterEvent(dsSTRID("OnLevelReloadRequest", false));
        }

        dsSTATIC_PARAM_LIST_2 params;
        params.Add("map", dbg_defaultLevel.value);
        params.Add("checkpoint", dbg_defaultChpt.value);
        gEventMgr->SignalEvent(evid_OnLevelReloadRequest, &params);
    }

    hcex_inside_processframe = true;
    hcex_process_frame();
    hcex_inside_processframe = false;

    hcex_pix_begin_event("hcex_update_anim");
    float elapsedTime = GetElapsedTime();
    hcex_update_anim(elapsedTime);
    hcex_cine_update();
    if ( hcex_enable_pix )
        osPIXEndEvent();

    hcex_pix_begin_event("frame_misc");
}
