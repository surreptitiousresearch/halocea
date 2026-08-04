#pragma once
#include "../os/osLOCK.h"
#include "../m3d/m3dRECT.h"
#include "../m3d/m2dV.h"
#include "../ds/dsVECTOR.h"
#include "UI_STAT_FONT.h"
#include "UI_STAT_RENDER.h"
#include "UI_STAT_ITEM.h"
#include "INGAME_ITEM.h"
#include "UI_STAT_PARAMS.h"
// ws-engine ui_new: the debug stat-overlay system — owns the two categories of debug text
// ("UI stat": a fixed top-of-screen counter list; "ingame": per-frame world/screen-anchored
// debug prints) and their glyph-box renderers.
// DB-verified layout (types_members ui_new::UI_STAT_SYS): lock@0 (osLOCK, 52B), font@52
// (UI_STAT_FONT, 48B), viewport@100 (m3dRECT, 16B), uiStatRenderer@116 (UI_STAT_RENDER, 44B),
// uiStatItems@160 (dsVECTOR<UI_STAT_ITEM,8>, 20B), ingameRenderer@180 (UI_STAT_RENDER, 44B),
// ingameItems@224 (dsVECTOR<INGAME_ITEM,8>, 20B), ingameItemsForRender@244
// (dsVECTOR<INGAME_ITEM,8>, 20B), ingameParams@264 (UI_STAT_PARAMS, 32B) — size 296.

namespace ui_new {

typedef void (__fastcall *UI_STAT_GET_PARAMS_CB)(UI_STAT_PARAMS &);
typedef void (__fastcall *UI_STAT_GET_DATA_CB)(dsTSTRING<wchar_t> &);

typedef struct UI_STAT_SYS {
    osLOCK                       lock;                 // 0x000
    UI_STAT_FONT                 font;                 // 0x034
    m3dRECT                      viewport;             // 0x064
    UI_STAT_RENDER                uiStatRenderer;       // 0x074
    dsVECTOR<UI_STAT_ITEM, 8>     uiStatItems;          // 0x0A0
    UI_STAT_RENDER                ingameRenderer;       // 0x0B4
    dsVECTOR<INGAME_ITEM, 8>      ingameItems;          // 0x0E0 back-buffer being filled this frame
    dsVECTOR<INGAME_ITEM, 8>      ingameItemsForRender; // 0x0F4 front-buffer being drawn this frame
    UI_STAT_PARAMS                ingameParams;         // 0x108 shared params for all ingame prints

    // 0x82CED340 — reversed in UI_STAT_SYS__AdjustStringPos.cpp. Protected (AAA mangle).
    m2dV AdjustStringPos(m2dV pos, int lineLength, char alignment, float scale, float interval);

    // 0x82CED448 — reversed in UI_STAT_SYS__GetLinesNumber.cpp. Protected (AAA mangle).
    int GetLinesNumber(const wchar_t *str);

    // 0x82CED99C — reversed in UI_STAT_SYS__GetPosFromViewport.cpp. Protected (AAA mangle).
    m2dV GetPosFromViewport(const wchar_t *str, char alignment, float scale);

    // 0x82CEEF40 — reversed in UI_STAT_SYS__ProcessRenderPush.cpp.
    void ProcessRenderPush();

    // 0x82CEFFA8 — reversed in UI_STAT_SYS__PrintLine.cpp. Protected (AAA mangle).
    void PrintLine(UI_STAT_RENDER &renderer, const wchar_t *str, const UI_STAT_PARAMS &params,
                    int lineLength, m2dV pos);

    // 0x82CF01D8 — reversed in UI_STAT_SYS__Print.cpp. Protected (AAA mangle).
    void Print(UI_STAT_RENDER &renderer, m2dV pos, const wchar_t *str, const UI_STAT_PARAMS &params);

    // 0x82CF02FC — reversed in UI_STAT_SYS__RenderUIStat.cpp.
    void RenderUIStat();

    // 0x82CF0554 — reversed in UI_STAT_SYS__RenderIngameStat.cpp.
    void RenderIngameStat();

    // 0x82CED504-ish — recompute `viewport` from the current backbuffer/UI-page rect. Not
    // reversed in this batch (only called by UI_FRAME_MGR-side setup code, not by any target in
    // this batch). boundary.
    void UpdateViewport(const m2dV &rtSize);

    // 0x82CED4B8-ish — measure the pixel size of `str` at `scale`/`interval` (used by callers
    // that need to lay out a box around printed text). Not reversed in this batch — not called
    // by any of this batch's targets. boundary. Protected (AAA mangle).
    m2dV GetStringSize(const wchar_t *str, float scale, float interval);

    // ??0UI_STAT_SYS@ui_new@@QAA@ABV?$dsTSTRING@D@@@Z — construct, loading `fontName`'s
    // UI_STAT_FONT. Not reversed in this batch. boundary.
    UI_STAT_SYS(const dsTSTRING<char> &fontName);

    // Register a UI-stat line's param/data callbacks (UI_STAT_ITEM.h callback typedefs; spelled
    // as typedefs so the declaration does not read as function-pointer data members). Not
    // reversed in this batch. boundary.
    void RegisterUIStatItem(UI_STAT_GET_PARAMS_CB getParams, UI_STAT_GET_DATA_CB getData);

    // Queue one ingame debug-print for this frame. Not reversed in this batch. boundary.
    void IngamePrintf(int x, int y, const dsTSTRING<char> &text);
} UI_STAT_SYS;

} // namespace ui_new
