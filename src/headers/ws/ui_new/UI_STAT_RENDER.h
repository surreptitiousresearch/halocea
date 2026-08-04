#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../rend/rendV3D.h"
#include "../txm/txmTEXTURE_PTR.h"
#include "../m3d/m3dRECT.h"
// ws-engine ui_new: a batched immediate-mode textured-quad renderer used by UI_STAT_SYS to draw
// glyph boxes (letters, drop shadows, and background rects) into one draw call per Flush.
// DB-verified layout (types_members ui_new::UI_STAT_RENDER): vBuf@0 (dsVECTOR<rendV3D,8>, 20B),
// iBuf@20 (dsVECTOR<unsigned short,8>, 20B), pTex@40 (txmTEXTURE_PTR, 4B) — size 44.

namespace ui_new {

typedef struct UI_STAT_RENDER {
    dsVECTOR<rendV3D, 8>        vBuf; // 0x00 quad vertex scratch buffer
    dsVECTOR<unsigned short, 8> iBuf; // 0x14 quad index scratch buffer (2 tris / box)
    txmTEXTURE_PTR              pTex; // 0x28 glyph-atlas texture (also gates every method below —
                                        //      un-set means "not yet InitTexture'd")

    // 0x82CEDC7C — reversed in UI_STAT_RENDER__Flush.cpp.
    void Flush();
    // 0x82CED900 — reversed in UI_STAT_RENDER__Reset.cpp.
    void Reset();
    // 0x82CEF5F8 — reversed in UI_STAT_RENDER__AddBox.cpp. NOTE: `uv` is passed BY VALUE per the
    // DB mangle (...ABUm3dRECT@@U3@@Z: rect is const&, uv is a plain m3dRECT by value).
    void AddBox(unsigned int clr, const m3dRECT &rect, m3dRECT uv);

    // 0x82CED424-ish — binds pTex from a texture name (loads/refs it via txmManager). Not
    // reversed in this batch; called by UI_STAT_SYS::RenderUIStat/RenderIngameStat. boundary.
    bool InitTexture(const dsTSTRING<char> &textureName);
} UI_STAT_RENDER;

} // namespace ui_new
