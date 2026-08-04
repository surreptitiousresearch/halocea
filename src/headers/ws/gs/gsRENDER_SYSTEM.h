#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/CALLBACK_MNG.h"
#include "../ap/apSTATE_T.h"
// ws-engine gs: the top-level Blam-bridge render system (per-frame swap/screenshot/HW-state
// housekeeping that wraps the legacy Blam rasterizer output for the `ws` render job pipeline).
// DB-verified layout (types_members gsRENDER_SYSTEM): nearPlane@0, farPlane@4, pFlareManager@8,
// pShadowMapSystem@12, screenShotPath@16 (dsTSTRING<char>), pEffectProcessor@20,
// pTexLoResDepthHalf@24, pTexLoResDepthQuater@28, pTexLoResDepthQuaterMax@32, pTexDuDv@36,
// pTexSMMask@40, screenShotNmb@44, skipFrameNmb@48, needTurnSplit@52, state@56
// (apSTATE_T<unsigned long>), onPrepareFrame@60 (ds::CALLBACK_MNG<void(*)()>, 20B),
// isCinematicMode@80, isStereo3D@81, stereo3DRatio@84, isNeedResetHW@88, isNeedUpdateVis@89,
// isNeedUpdateMaterials@90 — size 92.

struct gsFLARE_MNG_INTERFACE; // boundary — gs subsystem, only used by pointer here
struct gsSHADOW_MAP_SYSTEM;   // boundary — gs subsystem, only used by pointer here
struct gsEFFECT_PROCESSOR;    // boundary — gs subsystem, only used by pointer here
struct txmTEXTURE;            // boundary — txm subsystem, only used by pointer here
struct camCAMERA;             // boundary — cam subsystem, only used by pointer here

typedef struct gsRENDER_SYSTEM {
    float                     nearPlane;               // 0x00
    float                     farPlane;                // 0x04
    gsFLARE_MNG_INTERFACE    *pFlareManager;            // 0x08
    gsSHADOW_MAP_SYSTEM      *pShadowMapSystem;         // 0x0C
    dsTSTRING<char>           screenShotPath;           // 0x10
    gsEFFECT_PROCESSOR       *pEffectProcessor;         // 0x14
    txmTEXTURE               *pTexLoResDepthHalf;       // 0x18
    txmTEXTURE               *pTexLoResDepthQuater;     // 0x1C
    txmTEXTURE               *pTexLoResDepthQuaterMax;  // 0x20
    txmTEXTURE               *pTexDuDv;                 // 0x24
    txmTEXTURE               *pTexSMMask;               // 0x28
    int                       screenShotNmb;            // 0x2C incremented on each screenshot saved
    int                       skipFrameNmb;              // 0x30
    bool                      needTurnSplit;             // 0x34
    unsigned char             _pad35[3];                 // 0x35 db-verified padding
    apSTATE_T<unsigned long>  state;                    // 0x38 — bit 0x08 = screenshot requested,
                                                             //        bit 0x80 = "sticky" (don't
                                                             //        auto-clear the request bit)
    ds::CALLBACK_MNG<void(*)(void)> onPrepareFrame;      // 0x3C (20B)
    bool                      isCinematicMode;          // 0x50
    bool                      isStereo3D;               // 0x51
    unsigned char             _pad52[2];                 // 0x52 db-verified padding
    float                     stereo3DRatio;            // 0x54
    bool                      isNeedResetHW;            // 0x58
    bool                      isNeedUpdateVis;          // 0x59
    bool                      isNeedUpdateMaterials;    // 0x5A

    // 0x827A95AC — reversed in gsRENDER_SYSTEM__SwapBuffers.cpp.
    void SwapBuffers(int doActualSwap, int callerID);

    // 0x827A89AC — reversed in gsRENDER_SYSTEM___SaveScreenShot.cpp. Protected (IAA mangle);
    // `pCam` is accepted but unused by the body reconstructed in this batch (every call site in
    // the corpus passes nullptr).
    void _SaveScreenShot(camCAMERA *pCam);

    // 0x82xxxxxx — request a screenshot be saved on the next SwapBuffers (sets state bit 0x08).
    // boundary (outside this re-source); not called by any of this batch targets.
    void SaveScreenShot();
    // boundary (outside this re-source).
    void SaveScreenShotEveryFrame();

    // 0x827A1D20 — flip stereo-3D on/off and flag a visibility re-eval. Reversed in
    // gsRENDER_SYSTEM__ToggleStereo3D.cpp.
    void ToggleStereo3D();
    // 0x823CDD08 — const getter for the stereo-3D enable flag. Reversed in
    // gsRENDER_SYSTEM__IsStereo3D.cpp.
    bool IsStereo3D() const;
    // 0x823CDD10 — set the stereo-3D eye-separation ratio. Reversed in
    // gsRENDER_SYSTEM__SetStereo3DRatio.cpp.
    void SetStereo3DRatio(float ratio);
} gsRENDER_SYSTEM;
