/* hcex_render_hud @0x823D2A28 — HCEX bridge per-frame HUD/scene draw hook. Updates the stereo-3D
 * crosshair depth offset every frame regardless of mode; while the HUD isn't debug-disabled and the
 * game is active, refreshes the HALO render targets for the current split-screen layout and, in Halo
 * mode with the render pipeline ready, drives one or two Blam camera passes (prepare camera, screen
 * flash, HUD/interface draw, screen-flash overlay) under hcexHaloLogic, followed by a final shared
 * pass (cinematic overlay, HUD timer, debug terminal, FX-texture release) and a D3D render-state reset.
 *
 * DEVIATIONS (verified against disassembly):
 *  1. interface_draw_screen(), rasterizer_screen_flash(), cinematic_render(), hud_render_timer(),
 *     terminal_draw() and raserizer_release_fx_textures() all take NO arguments and their return
 *     values (where any) are unused — the raw decompile chained each call's r3 result into the next
 *     call's argument purely as a register-reuse artifact, not real data flow.
 *  2. vidDRIVER::SetViewport's 3rd/4th parameters are the opposite corner (x1, y1), not a width/height
 *     pair — see hcex_render_hud_boundary.h. The split-screen bottom-half viewport is therefore
 *     (x0=0, y0=sy/2, x1=sx, y1=sy), i.e. the bottom half of the screen, not an out-of-bounds rect. */

#include "../headers/hcex/hcex_render_hud_boundary.h"

void hcex_render_hud(void)
{
    crosshairOffsetX = ((dbg_crosshairDepth.value - g_Stereo3DUIConstants[1]) / dbg_crosshairDepth.value)
                     * g_Stereo3DUIConstants[0];

    if ( dbg_hcex_off_hud.value || !game_is_active() )
        return;

    unsigned int state = txmManager->state.state;
    int splitScreenIdx = (state >> 16) & 1;
    if ( ((state >> 15) & 1) == 0 )
        splitScreenIdx = -1;
    hcex_update_render_targets(splitScreenIdx);

    if ( !hcex_cur_render_mode )
        return;

    osLOCK__Lock(&hcexHaloLogic, "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 428);

    if ( hcex_render_ready )
    {
        vidDRIVER_INTERFACE__EnsureVertShader(vidDriver->drvInterface, 0);
        vidDRIVER_INTERFACE__EnsurePixShader(vidDriver->drvInterface, 0, 0);
        vidDRIVER_INTERFACE__EnsureShaderDecl(vidDriver->drvInterface, 0, 0, 0);
        vidDRIVER_INTERFACE__ClearSource(vidDriver->drvInterface);

        if ( hcex_coop_local_player_index < 0 && local_player_count() > 1 && !gsSysRender->isStereo3D )
            vidDRIVER__SetViewport(vidDriver, 0, 0, vidDriver->pMode->sx, vidDriver->pMode->sy / 2, 0.0f, 1.0f);

        hcex_prepare_halo_render(0);
        hcex_update_screen_flash(0);
        interface_draw_screen();
        if ( !hcex_off_cine_screen_effect || !hcex_is_cine_mode() )
            rasterizer_screen_flash();

        if ( hcex_coop_local_player_index < 0 && local_player_count() > 1 )
        {
            vidDRIVER__SetViewport(vidDriver, 0, vidDriver->pMode->sy / 2, vidDriver->pMode->sx,
                                   vidDriver->pMode->sy, 0.0f, 1.0f);
            hcex_prepare_halo_render(1);
            hcex_update_screen_flash(1);
            interface_draw_screen();
            rasterizer_screen_flash();
        }

        vidDRIVER__SetViewport(vidDriver, 0, 0, vidDriver->pMode->sx, vidDriver->pMode->sy, 0.0f, 1.0f);
        hcex_prepare_halo_render(-1);
        hcex_update_screen_flash(-1);
        cinematic_render();
        hud_render_timer();
        terminal_draw();
        raserizer_release_fx_textures();
        d3dDRIVER_SetStartRenderingState(d3dDriver);
    }

    osLOCK_Unlock(&hcexHaloLogic, "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 428);
}
