#pragma once
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dV.h"
// ws-engine ho/gm debug: one recorded ray-query failure (used by hoEDGE_DBG_INFO and
// gmHIDEOUT_DEBUGGER). DB-verified layout (types_members RAY_RELATED_ERROR) — size 84 (0x54):
// strError@0, strCdtObj@4, errorCdtVertices@8 (m3dV[3]), vFrom@44, vTo@56, vClos@68, hasRay@80.

struct RAY_RELATED_ERROR {
    dsTSTRING<char> strError;            // 0x00
    dsTSTRING<char> strCdtObj;           // 0x04
    m3dV            errorCdtVertices[3]; // 0x08
    m3dV            vFrom;               // 0x2C
    m3dV            vTo;                 // 0x38
    m3dV            vClos;               // 0x44
    bool            hasRay;              // 0x50
    // 3 bytes padding to 0x54
};
