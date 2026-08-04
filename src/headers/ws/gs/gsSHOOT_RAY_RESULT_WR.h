#pragma once
#include "../m3d/m3dV.h"
// ws-engine gs: cached result of a ShootRay collision query. DB-verified layout
// (types_members gsSHOOT_RAY_RESULT_WR): bRes@0, pObj@4, vClosest@8, idFace@20,
// vClosestLSeg@24, distMin@36 — size 40 (0x28).

struct objOBJ; // ws obj — hit object (fwd, boundary pointer)

struct gsSHOOT_RAY_RESULT_WR {
    bool    bRes;         // 0x00
    // 3 bytes padding to 0x04
    objOBJ *pObj;         // 0x04
    m3dV    vClosest;     // 0x08
    int     idFace;       // 0x14
    m3dV    vClosestLSeg; // 0x18
    float   distMin;      // 0x24
};
