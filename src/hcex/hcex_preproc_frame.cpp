/* hcex_preproc_frame @0x823DF080 — runs once at the top of the HCEX per-frame update: destroys any
 * scene instances queued for deferred removal, updates render/kinect mode, applies the debug
 * "force local-player index" coop toggle (swaps between local players 0 and 1), and reconciles the
 * ws-engine ENGINE_CONTROL's split-screen flag with hcex_split_screen while outside a cinematic in
 * legacy (Blam) render mode. */

#include "../headers/hcex/hcex_preproc_frame_boundary.h"

void hcex_preproc_frame(void)
{
    hcex_pix_begin_event("hcex_preproc_frame");

    hcex_destroy_dead_intances();
    hcex_change_render_mode();
    hcex_change_kinect_state();

    if ( dbg_hcex_test_online_coop_switch_plr.value )
    {
        if ( hcex_coop_local_player_index == 0 )
            hcex_coop_local_player_index = 1;
        else if ( hcex_coop_local_player_index == 1 )
            hcex_coop_local_player_index = 0;
    }

    bool is_split = ENGINE_CONTROL_IsSplitScreen(gsEngineCtrl);

    if ( !cinematic_in_progress() && hcex_cur_render_mode && hcex_split_screen )
    {
        if ( !is_split )
            ENGINE_CONTROL_SetSplitScreen(gsEngineCtrl, 1);
    }
    else if ( !is_split )
    {
        /* deviation: reproduced faithfully — this call is a no-op (is_split is already false here). */
        ENGINE_CONTROL_SetSplitScreen(gsEngineCtrl, 0);
    }

    if ( hcex_enable_pix )
        osPIXEndEvent();
}
