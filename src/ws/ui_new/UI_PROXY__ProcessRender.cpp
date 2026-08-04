#include "../../headers/ws/ui_new/UI_PROXY.h"
#include "../../headers/ws/ui_new/ui_proxy_boundary.h"
#include "../../headers/ws/ui_new/ui_stat_boundary.h"
#include "../../headers/ws/ui_new/gfx_render_stats.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h" // apCOUNTER_TIME_CALL_WRP<> RAII scope timer
#include "../../headers/hcex/haloInit_boundary.h" // UI_EXT_RENDER

using ui_new::gfx_PrintRenderStats;
using ui_new::gfx_ResetRenderStats;

// Function-local static timer counter (see UI_STAT_SYS::RenderUIStat for the pattern).
static apCOUNTER_TIME_CALL localTimeCounter_ProcessRender("ui/new/UI_PROXY::ProcessRender");

// 0x82780C00 — per-frame UI render: draw ingame debug stats, run the (Scaleform GFx) UI page
// render stats bracket, invoke the game-side UI_EXT_RENDER hook if bound, run the frame-manager's
// own render, then draw the top-of-screen UI stat overlay text.
void ui_new::UI_PROXY::ProcessRender()
{
    osPIXBeginEvent("UI_PROXY::ProcessRender");

    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> timerScope(&localTimeCounter_ProcessRender);

    if (isEnabled)
    {
        if (uiStatSys)
            uiStatSys->RenderIngameStat();

        if (wb::IsActive(uiPage))
            gfx_ResetRenderStats(uiPage);

        if (UI_EXT_RENDER)
        {
            UI_EXT_RENDER();
            vidDriver->drvInterface->SetGammaCorrection(0);
        }

        if (uiImpl)
            uiImpl->ProcessRender();

        if (wb::IsActive(uiPage))
            gfx_PrintRenderStats(uiPage);

        vidDriver->SetScissorRect(0, 0, vidDriver->pMode->sxRT, vidDriver->pMode->syRT);

        if (uiStatSys)
        {
            osPIXBeginEvent("Counter's text");
            uiStatSys->RenderUIStat();
            osPIXEndEvent();
        }
    }

    osPIXEndEvent();
}
