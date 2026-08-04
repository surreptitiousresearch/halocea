#pragma once
#include "../msg/msgADDR.h"
#include "../m3d/m2dV.h"
#include "../ds/dsVECTOR.h"
#include "../os/osLOCK.h"
#include "../ssl/sslOBJ_REF.h"
#include "UI_FRAME.h"
#include "UI_FRAME_MGR.h"
#include "UI_STAT_SYS.h"
// ws-engine ui_new: the top-level UI bridge object — owns the frame-manager (menus/HUD), the
// mouse cursor, the debug stat overlay, and the (optional) loading-screen frame, and drives their
// per-frame render push/process.
// DB-verified layout (types_members ui_new::UI_PROXY): msgADDR (base)@0 (32B),
// bLoadingScreenActive@32 (bool), uiImpl@36 (UI_FRAME_MGR*), uiMouse@40 (UI_MOUSE*), uiStatSys@44
// (UI_STAT_SYS*), uiLoadingScreen@48 (UI_FRAME*), allowLoadingScreen@52 (bool),
// loadingScreenProgress@56 (float), isEnabled@60 (bool), isSplitScreen@61 (bool),
// isViewportInitialized@62 (bool), viewPortSize@64 (m2dV), uiPage@72 (int), ingameFrames@76
// (dsVECTOR<UI_PROXY::INGAME_FRAME_REC,8>, 20B), ingameFramesLock@96 (osLOCK, 52B), sslObject@148
// (sslOBJ_REF, 4B) — size 152.

namespace ui_new {

struct UI_MOUSE; // boundary — ui_new subsystem, only used by pointer here

struct UI_PROXY : public msgADDR {
    // One registered ingame-debug-print SSL/frame entry. Not touched by this batch's targets —
    // forward-declared only so `ingameFrames`'s element type is nameable. boundary.
    struct INGAME_FRAME_REC;

    bool               bLoadingScreenActive;   // 0x020
    unsigned char      _pad0[3];                // 0x021 db-verified padding
    UI_FRAME_MGR       *uiImpl;                 // 0x024
    UI_MOUSE           *uiMouse;                 // 0x028
    UI_STAT_SYS        *uiStatSys;               // 0x02C
    UI_FRAME           *uiLoadingScreen;         // 0x030
    bool               allowLoadingScreen;      // 0x034
    unsigned char      _pad1[3];                // 0x035 db-verified padding
    float              loadingScreenProgress;   // 0x038
    bool               isEnabled;               // 0x03C
    bool               isSplitScreen;           // 0x03D
    bool               isViewportInitialized;   // 0x03E
    unsigned char      _pad2[1];                // 0x03F db-verified padding
    m2dV               viewPortSize;            // 0x040
    int                uiPage;                  // 0x048 wb debug-page index for render-stat text
    dsVECTOR<INGAME_FRAME_REC, 8> ingameFrames;  // 0x04C
    osLOCK             ingameFramesLock;         // 0x060
    sslOBJ_REF         sslObject;                 // 0x094

    // 0x82780C00 — reversed in UI_PROXY__ProcessRender.cpp.
    void ProcessRender();
    // 0x82780B50 — reversed in UI_PROXY__RenderPush.cpp.
    void RenderPush();
};

} // namespace ui_new
