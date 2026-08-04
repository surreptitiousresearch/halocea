#pragma once
#include "msg/msgADDR.h"
#include "ds/dsSMART_PTR.h"
#include "ssl/sslOBJ_REF.h"
#include "m3d/m3dMATR.h"
// ws-engine control object (ENGINE_CONTROL). Full DB-verified layout (types_members
// ENGINE_CONTROL, reconciled 2026-08-04): msgADDR base (32B) + members below — size 108 (0x6C).
// (Distinct from the hcex haloENGINE_CONTROL bridge object; this is the plain ws ENGINE_CONTROL.)

struct dscDESC;     // dsc descriptor (pointer only)
struct dsREF_COUNT; // ds intrusive refcount base (policy parameter only)

struct ENGINE_CONTROL : msgADDR {
    dsSMART_PTR<dscDESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDesc; // 0x20
    sslOBJ_REF    sslObject;           // 0x24
    bool          isMultiplayer;       // 0x28
    bool          isSplitScreen;       // 0x29 set while rendering in split-screen
    unsigned char _pad2A[2];           // 0x2A db-verified padding
    m3dMATR       destroySavePosition; // 0x2C (64B)
    // 108 bytes

    // ?IsSplitScreen@ENGINE_CONTROL@@QAA_NXZ @ 0x82768550 -- getter for isSplitScreen.
    bool IsSplitScreen();

    // ?SetSplitScreen@ENGINE_CONTROL@@QAAX_N@Z @ 0x82768560 -- toggle split-screen: reconfigure
    // cameras, flag a visibility update, and signal the "OnSplitScreen" event.
    // REVERSED: src/ws/gs/ENGINE_CONTROL__SetSplitScreen.cpp
    void SetSplitScreen(bool isOn);
};
