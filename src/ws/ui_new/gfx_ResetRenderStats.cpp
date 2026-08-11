// FUNCTION_INDEX entry: gfx_ResetRenderStats @0x833188D0 (?gfx_ResetRenderStats@ui_new@@YAXH@Z)
#include "../../headers/ws/ui_new/GFX_SYSTEM.h"
#include "../../headers/ws/ui_new/gfx_render_stats.h"
#include "../../headers/ws/ui_new/ui_stat_boundary.h"

// 0x83318xxx (?gfx_ResetRenderStats@ui_new@@YAXH@Z) — reset the GFx renderer's per-frame draw-
// call counters and clear the debug page's accumulated text, in preparation for GetCountersString
// tallying this frame's draws (see gfx_PrintRenderStats).
void ui_new::gfx_ResetRenderStats(int page)
{
    if (gGfxSystem)
    {
        gGfxSystem->pRenderer->ResetFrameCounters();
        wb::Clear(page);
    }
}
