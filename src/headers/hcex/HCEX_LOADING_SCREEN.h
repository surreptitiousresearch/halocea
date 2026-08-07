#pragma once
#include "../ws/ui_new/UI_FRAME.h"
// HCEX bridge: the concrete ui_new::UI_FRAME implementing the HCEX loading screen (background +
// mask texture blend, animated progress bar). Registered as the "hcex_loading_screen" brand via
// Register(). DB-verified layout (types_members HCEX_LOADING_SCREEN): ui_new::UI_FRAME (base)@0
// (84B), loadingScreenProgress@84 (float) — size 88.
// DB-verified vtable layout (types_members HCEX_LOADING_SCREEN_vtbl) — 20 slots; only the ones
// this batch's methods reference are named precisely, the rest are declared from the DB
// prototypes for completeness (bodies boundary, not decompiled here).

struct psSECTION;     // boundary — ps subsystem
#include "../ws/ds/dsSTRID.h" // interned string id — ReportStat's first slot parameter, by value
struct dsPARAM_LIST;  // boundary — ds subsystem
struct m2dV;           // boundary — m2d subsystem
struct m3dRECT;        // boundary — m3d subsystem
struct HCEX_LOADING_SCREEN;

typedef struct HCEX_LOADING_SCREEN_vtbl {
    void (*dtr_HCEX_LOADING_SCREEN)(HCEX_LOADING_SCREEN *self, int freeMemory);                                // 0x00 — deleting dtor: vftable+0x00 holds ??_GHCEX_LOADING_SCREEN@@UAAPAXI@Z
    void (*ParsePS)(HCEX_LOADING_SCREEN *self, psSECTION *section);                             // 0x04
    void (*Init)(HCEX_LOADING_SCREEN *self);                                                    // 0x08
    void (*Term)(HCEX_LOADING_SCREEN *self);                                                    // 0x0C
    void (*UpdateViewport)(HCEX_LOADING_SCREEN *self, const m2dV *, const m3dRECT *, unsigned int); // 0x10
    void (*ProcessRender)(HCEX_LOADING_SCREEN *self);                                           // 0x14
    void (*ProcessKey)(HCEX_LOADING_SCREEN *self);                                              // 0x18
    void (*ProcessMouse)(HCEX_LOADING_SCREEN *self, dsPARAM_LIST *);                             // 0x1C
    void (*ProcessCall)(HCEX_LOADING_SCREEN *self, const char *methodName, const dsPARAM_LIST *paramList); // 0x20
    void (*ProcessFrame)(HCEX_LOADING_SCREEN *self);                                             // 0x24
    void (*ProcessUIThread)(HCEX_LOADING_SCREEN *self);                                           // 0x28
    /* ?ReportStat@UI_FRAME@ui_new@@UAAXVdsSTRID@@PBD@Z -- dsSTRID BY VALUE (`V`), not an int. */
    void (*ReportStat)(HCEX_LOADING_SCREEN *self, dsSTRID statId, const char *);                   // 0x2C
    bool (*IsReady)(HCEX_LOADING_SCREEN *self);                                                   // 0x30
    void (*Enable)(HCEX_LOADING_SCREEN *self);                                                    // 0x34
    void (*Disable)(HCEX_LOADING_SCREEN *self);                                                   // 0x38
    void (*LockToPrimary)(HCEX_LOADING_SCREEN *self, bool);                                       // 0x3C
    void (*SetFocus)(HCEX_LOADING_SCREEN *self, bool);                                            // 0x40
    void (*SslInit)(HCEX_LOADING_SCREEN *self);                                                   // 0x44
    void (*SslDestroy)(HCEX_LOADING_SCREEN *self);                                                 // 0x48
    void (*SslReload)(HCEX_LOADING_SCREEN *self);                                                  // 0x4C
} HCEX_LOADING_SCREEN_vtbl;

struct HCEX_LOADING_SCREEN : ui_new::UI_FRAME {
    static HCEX_LOADING_SCREEN_vtbl vftable; // `HCEX_LOADING_SCREEN::`vftable'' — boundary, not decompiled

    float loadingScreenProgress; // 0x54 (84) — last value set via ProcessCall("OnLoadingProgressChanged")

    // 0x823F3A80 — chain to ui_new::UI_FRAME's ctor, set this class's own vtable, zero
    // loadingScreenProgress, then create the shared D3D resources (shaders/textures/vertex
    // declaration) if not already created.
    HCEX_LOADING_SCREEN();
    // 0x823F3B20 — reset __vftable to this class's own (defends against a base dtor already
    // having stomped it), then chain to ~ui_new::UI_FRAME.
    ~HCEX_LOADING_SCREEN();
    // 0x823F41D8 — scalar deleting destructor: run the dtor chain, then `operator delete` this
    // object if the low bit of `freeMemory` is set.
    HCEX_LOADING_SCREEN *ScalarDeletingDestructor(unsigned int freeMemory);

    // 0x823F3B30 — per-frame render: gamma-correct the render target, ease the visible progress
    // value toward loadingScreenProgress (adaptive frame-rate-independent lerp), bind the
    // background/mask/resolve textures with clamp addressing, and draw the fullscreen progress
    // quad. Not reversed field-by-field beyond the visible math -- see the CAVEAT in the .cpp for
    // the raw GPU sampler-state / vertex-shader-constant pokes this collapses.
    void ProcessRender();

    // 0x823F3F08 — chain to ui_new::UI_FRAME::Enable, then reset the visible-progress animation
    // state (progress=0, velocity=0.025, clear-prev-frame flag set, session start/last-update
    // timestamps taken from the current tick count).
    void Enable();

    // 0x823F44E8 — the only recognized SSL call is "OnLoadingProgressChanged", which reads a
    // float out of paramList[0] into loadingScreenProgress.
    void ProcessCall(const char *methodName, const dsPARAM_LIST *paramList);

    // 0x823F4880 — register the "hcex_loading_screen" brand (parented to "ui_iface") with
    // ui_new::uiFamily, and set its constructor thunk to dsMakeFunc<void,HCEX_LOADING_SCREEN>.
    static void Register();
};
