/* ?RenderPush@UI_PROXY@ui_new@@QAAXXZ @0x82780B20 */
#include "../../headers/ws/ui_new/UI_PROXY.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h" // apCOUNTER_TIME_CALL_WRP<> RAII scope timer

// Function-local static timer counter (see UI_STAT_SYS::RenderUIStat for the pattern).
static apCOUNTER_TIME_CALL localTimeCounter_RenderPush("ui/new/UI_PROXY::RenderPush");

// 0x82780B50 — per-frame UI render-push: while enabled and no (enabled) loading-screen frame is
// covering the UI, push the stat overlay's queued draw state (glyph-box batches) for this frame.
void ui_new::UI_PROXY::RenderPush()
{
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> timerScope(&localTimeCounter_RenderPush);

    if (isEnabled)
    {
        if (!uiLoadingScreen || !uiLoadingScreen->IsEnabled())
        {
            if (uiStatSys)
                uiStatSys->ProcessRenderPush();
        }
    }
}
