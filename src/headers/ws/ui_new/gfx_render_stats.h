#pragma once
// ws-engine ui_new: debug-page hooks for the Scaleform GFx renderer's per-frame draw-call
// statistics (bracket a UI page's render with gfx_ResetRenderStats/gfx_PrintRenderStats to show
// its GFx draw-call cost on that debug page).

namespace ui_new {

// 0x83318xxx — reversed in gfx_ResetRenderStats.cpp.
void gfx_ResetRenderStats(int page);
// 0x8331C774 — reversed in gfx_PrintRenderStats.cpp.
void gfx_PrintRenderStats(int page);

} // namespace ui_new
