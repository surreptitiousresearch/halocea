#pragma once
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"
#include "../gs/gsSHOOT_RAY_RESULT_WR.h"
// ws-engine debug ray collector (embedded in hoEDGE_DBG_INFO). DB-verified layout
// (types_members rayCOLLECTOR): rays@0 (dsVECTOR<rayCOLLECTOR::RAY,8>, 20), lastRayInfo@20
// (gsSHOOT_RAY_RESULT_WR, 40) — size 60 (0x3C).

struct rayCOLLECTOR {
    // DB-verified (types_members rayCOLLECTOR::RAY): p1@0, p2@12, color@24 — size 28.
    struct RAY {
        m3dV         p1;    // 0x00
        m3dV         p2;    // 0x0C
        unsigned int color; // 0x18
    };

    dsVECTOR<RAY, 8>      rays;        // 0x00
    gsSHOOT_RAY_RESULT_WR lastRayInfo; // 0x14
};
