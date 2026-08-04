#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/ds_assert_boundary.h"
#include "../gs/gsSTRINGS.h"
#include "../m3d/m3dCOLOR.h"
// Shared boundary declarations for the ui_new debug stat-overlay (ui_stat.cpp) reconstructions in
// this batch: the wb debug-window free functions they call but do not themselves reverse, the
// dbgVAR-style debug toggles that gate ingame-text rendering, and misc module-local statics.

namespace wb {
// wb::IsActive(int page) — true when the given debug page is currently visible/enabled.
// boundary (same-namespace, external to this batch — see wb_boundary.h for its siblings).
bool IsActive(int page);
// wb::Printf(int page, const char *fmt, ...) — append formatted text to a debug page's buffer.
// boundary.
void Printf(int page, const char *fmt, ...);
// wb::Clear(int page) — clear a debug page's accumulated text. boundary.
void Clear(int page);
} // namespace wb

// Generic "value + previous value" debug-toggle wrapper, matching the corpus's DBG_VAR<T>
// convention used throughout the dbg* globals (see e.g.
// HALO_SOUND_LIST_HALO_CHANNEL_globals.h).
template<class T>
struct DBG_VAR {
    T value;
    T prevValue;
};

// Debug toggles gating UI_STAT_SYS::ProcessRenderPush's ingame-text double-buffer swap.
extern DBG_VAR<bool>     dbgIngameTextEnable;
extern DBG_VAR<m3dCOLOR> dbgIngameTextColor;
extern DBG_VAR<float>    dbgIngameTextScale;
extern DBG_VAR<bool>     dbgIngameTextShadow;

// Module-local "renderer failed to init once, stop retrying this session" latches (ui_stat.cpp).
extern bool switchOff;    // UI_STAT_SYS::RenderUIStat
extern bool switchOff_0;  // UI_STAT_SYS::RenderIngameStat
