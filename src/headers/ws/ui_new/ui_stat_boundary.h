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

// Debug toggles gating UI_STAT_SYS::ProcessRenderPush's ingame-text double-buffer swap.
//
// DEVIATION (odr_dup drain): these were declared as a locally-defined `template<class T> struct
// DBG_VAR { T value; T prevValue; };`, one of three verbatim copies of that body at file scope
// (also hcex/HALO_SOUND_SYSTEM_globals.h and hcex/HALO_SOUND_LIST_HALO_CHANNEL_globals.h). It was
// also the wrong layout: `.value` sits at 0x0C behind a 12-byte dbgVAR base (__vftable@0, name@4,
// typeId@8 — types_members dbgVAR / dbgVAR_IMPL<T,N>), not at offset 0. Replaced by the canonical
// DB-verified templates; `.value` is inherited, so no consumer expression changes.
#include "../wb/dbgVAR_boundary.h"   // dbgVAR / dbgVAR_IMPL<T,N> / dbgVAR_SIMPLE<T,N>

// dbgVAR_M3DCOLOR — the DB's own type for dbgIngameTextColor (applied_types:
// `dbgVAR_M3DCOLOR dbgIngameTextColor;`). types_members dbgVAR_M3DCOLOR: base
// dbgVAR_SIMPLE<m3dCOLOR,9>@0 (60 bytes) + ranged@60 (bool) — DB `types` size 64. It adds the one
// field over the plain simple-var shape, so it is spelled out here rather than approximated.
struct dbgVAR_M3DCOLOR : dbgVAR_SIMPLE<m3dCOLOR, 9> {
    bool ranged; // 0x3C clamp the edited colour to the 0..1 range
};

// applied_types, one per symbol: dbgVAR_SIMPLE<bool,1> / dbgVAR_M3DCOLOR / dbgVAR_SIMPLE<float,3> /
// dbgVAR_SIMPLE<bool,1>.
extern "C" dbgVAR_SIMPLE<bool, 1>  dbgIngameTextEnable;
extern "C" dbgVAR_M3DCOLOR         dbgIngameTextColor;
extern "C" dbgVAR_SIMPLE<float, 3> dbgIngameTextScale;
extern "C" dbgVAR_SIMPLE<bool, 1>  dbgIngameTextShadow;

// Module-local "renderer failed to init once, stop retrying this session" latches (ui_stat.cpp).
extern "C" bool switchOff;    // UI_STAT_SYS::RenderUIStat
extern "C" bool switchOff_0;  // UI_STAT_SYS::RenderIngameStat
