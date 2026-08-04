/* hcex_render_present @0x823F0F48 — HCEX bridge end-of-frame present hook. It flushes the new UI proxy through
 * the ws render path (start-rendering state, begin/process/end scene, wrapped in a PIX event), then — only in
 * Halo mode with stereo-3D active — copies the back buffer into the split-screen render target and
 * re-composites it as two vertical half-screen quads (top eye / bottom eye) before swapping buffers.
 *
 * Every driver/manager/system it touches is a ws-engine boundary (see hcex_render_present_boundary.h).
 *
 * CAVEAT: reconstructed from a "local variable allocation has failed" decompile. The stereo-3D quad loop's
 * trailing RenderQuad arguments were recovered from the disassembly (@0x823F1060-0x823F1118): the 9th float
 * is 0.0 (f9), the colour mask is 0xFFFFFFFF (r28 = -1), the blend mode is 0 (r25), and the quad texture is
 * the same split-screen target r29 already holds from CopyBackBufferToTexture. Width = pMode->sx and each
 * quad spans half of pMode->sy vertically. */

#include "../headers/hcex/hcex_render_present_boundary.h"

extern "C" void hcex_render_present(void)
{
    if ( ui_new_uiProxy )
    {
        d3dDRIVER_SetStartRenderingState(d3dDriver);
        ui_new_UI_PROXY_RenderPush(ui_new_uiProxy);
        osPIXBeginEvent("Render_FlushUI_NEW");
        vidDRIVER_BeginScene(vidDriver);
        ui_new_UI_PROXY_ProcessRender(ui_new_uiProxy);
        vidDRIVER_EndScene(vidDriver);
        osPIXEndEvent();
    }

    if ( hcex_is_halo_mode() && gsSysRender->isStereo3D )
    {
        txmTEXTURE *split_tex = hcxe_global_render_targets_tex[1]->pSplitScreenTex[0];

        vidDRIVER_CopyBackBufferToTexture(vidDriver, split_tex, 0, 0, -1, -1);

        txmManager->state.state &= ~0x8000u;
        vidDRIVER_SetRenderTarget(vidDriver, 0, 0, 2);   /* apSTATE_T<ulong>{2} passed by value */

        float width      = (float)vidDriver->pMode->sx;
        int   halfHeight = vidDriver->pMode->sy / 2;
        int   top = 0;
        int   i;
        for ( i = 0; i < 2; ++i )
        {
            int bottom = top + halfHeight;
            rendDRIVER_RenderQuad(rendDrv,
                                  0.0f, (float)top, 0.0f, 0.0f,
                                  width, (float)bottom, 1.0f, 1.0f,
                                  0.0f, BLENDMODE_DEFAULT, 0xFFFFFFFFu, split_tex);
            top = bottom;
        }

        txmManager->state.state |= 0x8000u;
    }

    gsRENDER_SYSTEM_SwapBuffers(gsSysRender, 1, 0);
}
