/* hcex_change_render_mode @0x823DEE38 — commit a pending HCEX render-mode switch.
 *
 * When hcex_new_render_mode differs from the committed hcex_cur_render_mode, this stores the new
 * mode and reconfigures the pipeline for it under a PIX profiling event:
 *   - mode 1 (e.g. stereo/alternate): enable gamma correction, zero the driver brightness offset,
 *     enable PWL gamma, re-arm the D3D render state, and disable blam debug drawing suppression.
 *   - mode 0 (normal): disable gamma correction, apply a 0.15 brightness offset, disable PWL gamma.
 * In both cases it then clears+swaps the render-node manager (twice), clears active + previous-frame
 * lights, clears shadow-map lights, takes render-thread ownership, inserts and blocks on a GPU fence,
 * and drops the streamed-texture worklist. Switching back to mode 0 additionally walks every managed
 * texture — for each not already released (state bits 26/28) it resets its streaming resource's
 * removingMip to minMip and unloads the hardware resource — then re-precaches resources under a
 * blocking stream. Finally it releases render-thread ownership and ends the PIX event.
 *
 * DEVIATIONS from the raw decompile:
 *  - d3dDriver->brightnessOffset / ->isGammaChanged were emitted as direct d3dDRIVER members; they
 *    are actually vidDRIVER base members (offsets 0x04 / 0x324, DB-verified) — accessed via ->base.
 *  - The mode-0 branch emits an extra txmMANAGER::Begin() whose result is discarded (an unused
 *    iterator constructed into a stack slot); reproduced faithfully with a (void) cast.
 */

#include "../headers/hcex/hcex_change_render_mode_boundary.h"

void hcex_change_render_mode(void)
{
    if ( hcex_cur_render_mode == hcex_new_render_mode )
        return;

    osPIXBeginEvent("change_render_mode");
    hcex_cur_render_mode = hcex_new_render_mode;

    if ( hcex_new_render_mode == 1 )
    {
        hcex_render_targets_set_gamma(1);
        vidUseGammaCorrection = 1;
        d3dDriver->base.brightnessOffset = 0.0f;
        d3dDriver->base.isGammaChanged = 1;
        d3dDriver->enablePWLGamma = 1;
        d3dDriver->SetStartRenderingState();
        debug_no_drawing = 1;
    }
    else
    {
        hcex_render_targets_set_gamma(0);
        vidUseGammaCorrection = 0;
        d3dDriver->base.brightnessOffset = 0.15000001f;
        d3dDriver->base.isGammaChanged = 1;
        d3dDriver->enablePWLGamma = 0;
        (void)txmManager->Begin(); // discarded iterator — matches the binary
        debug_no_drawing = 0;
    }

    rnsMNG_ClearAndSwap();
    rnsMNG_ClearAndSwap();
    rendLgtMng->ClearActiveLights();
    rendLgtMng->PushLightsForRenderingPreviousFrame();
    if ( gsShadowMap )
        gsShadowMap->ClearLights();

    vidDriver->AcquireThreadOwnership();
    unsigned int fence = D3DDevice_InsertFence(d3dDriver->pDevice);
    D3DDevice_BlockOnFence(fence);
    strmRES_TEX_MANAGER::mng.Clear();

    if ( !hcex_cur_render_mode )
    {
        txmMANAGER::ITERATOR it = txmManager->Begin();
        while ( it.index >= 0 && it.index < it.listTex->nElem )
        {
            txmTEXTURE *tex = (*it.listTex)[it.index];
            unsigned int state = tex->state.state;
            if ( ((state >> 26) & 1) == 0 && ((state >> 28) & 1) == 0 )
            {
                strmRESOURCE_TEXTURE *res = tex->strmResource;
                if ( res )
                {
                    res->removingMip = res->minMip;
                    tex->UnloadResource();
                }
            }
            ++it;
        }
    }

    strmMemMng.Sync();
    strmMemMng.ReserveAllMemory();

    if ( !hcex_cur_render_mode )
    {
        hcex_tex_block = 1;
        hcex_precache_resources();
        hcex_tex_block = 0;
    }

    vidDriver->ReleaseThreadOwnership();
    osPIXEndEvent();
}
