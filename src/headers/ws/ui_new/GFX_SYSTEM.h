#pragma once
#include "../gfx/gfxRENDERER.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
#include "../ds/MAP.h"
#include "../os/osLOCK.h"
// ws-engine ui_new: owns the Scaleform GFx SDK objects (loader/movie tables/render config) and
// the gfx::gfxRENDERER backend. Only `pRenderer` (read by gfx_PrintRenderStats/
// gfx_ResetRenderStats) is modeled at its DB offset; the GFx SDK members are boundary padding.
// DB-verified layout (types_members ui_new::GFX_SYSTEM): pSystem@0, pGfxLoader@4,
// movieDefList@8 (84B), movieMap@92 (84B), pRenderer@176 (gfx::gfxRENDERER*), pRenderConfig@180,
// lock@184 (osLOCK, 52B) — size 236.

#include "../gfx/GPtr.h"

// Scaleform GFx SDK boundary types (third-party; pointer/GPtr targets only).
struct GFxSystem;
struct GFxLoader;
struct GFxMovieDef;
struct GFxMovieView;
struct GFxRenderConfig;

namespace ui_new {

// Full DB-verified layout (types_members ui_new::GFX_SYSTEM) — size 236.
typedef struct GFX_SYSTEM {
    GFxSystem        *pSystem;      // 0x00
    GFxLoader        *pGfxLoader;   // 0x04
    ds::MAP<dsSTRID, GPtr<GFxMovieDef>,  ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> movieDefList; // 0x08 (84B)
    ds::MAP<dsSTRID, GPtr<GFxMovieView>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> movieMap;     // 0x5C (84B)
    gfx::gfxRENDERER *pRenderer;    // 0xB0
    GFxRenderConfig  *pRenderConfig;// 0xB4
    osLOCK            lock;         // 0xB8 (52B)

    // 0x8331Cxxx-ish — format the current gfx::gfxCounters into a human-readable string.
    // boundary (body outside this re-source); called by ui_new::gfx_PrintRenderStats.
    void GetCountersString(dsTSTRING<char> *outText);
} GFX_SYSTEM;

// ?gGfxSystem@ui_new@@3PAVGFX_SYSTEM@1@A
extern GFX_SYSTEM *gGfxSystem;

} // namespace ui_new
