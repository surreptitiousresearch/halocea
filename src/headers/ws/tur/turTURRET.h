#pragma once
#include "../m3d/m3dMATR.h"
#include "../ent/entTRACKER.h"
#include "../prop/propENTCONTAINER.h"

// ws-engine tur subsystem — turret property. DB-verified layout (types_members turTURRET,
// re-checked 2026-08-04) — size 0x150, derives from propENTCONTAINER (0x50 base).

struct objOBJ;   // obj — scene object          boundary (ptr)
struct m3dV;

struct turTURRET : propENTCONTAINER {
    objOBJ  *pObjBase;                          // 0x50
    objOBJ  *pObjRotX;                          // 0x54
    objOBJ  *pObjRotY;                          // 0x58
    m3dMATR  iniRotX;                           // 0x5C
    m3dMATR  iniRotY;                           // 0x9C
    float    angleX;                            // 0xDC
    float    angleY;                            // 0xE0
    float    tgtAngleX;                         // 0xE4
    float    tgtAngleY;                         // 0xE8
    entTRACKER tgtTracker;                       // 0xEC
    bool     isProcessed;                       // 0x14C

    // Virtual/method: can this turret aim its barrel at `worldPoint`? boundary body.
    // ?CanAimAtPoint@turTURRET@@QBA_NABUm3dV@@@Z — const member, takes const m3dV& (DB mangling).
    bool CanAimAtPoint(const m3dV &worldPoint) const;
};
