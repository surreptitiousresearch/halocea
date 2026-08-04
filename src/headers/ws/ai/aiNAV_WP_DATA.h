#pragma once
// ws-engine ai08 — parsed nav-waypoint description consumed by aiNAV_WP_WRAPPER_TPL.
// DB-verified layout (types_members aiNAV_WP_DATA) — size 128 (0x80).

#include "../ds/dsTSTRING.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m3dV.h"

struct aiNAV_WP_DATA {
    dsTSTRING<char> name;            // 0x00
    m3dMATR         matrix;          // 0x04 placement matrix
    float           distReach;       // 0x44
    float           radiusHO;        // 0x48
    unsigned int    flags;           // 0x4C
    float           twrManStr;       // 0x50
    bool            haveMatrix;      // 0x54
    bool            haveDistReach;   // 0x55
    bool            haveRadiusHO;    // 0x56
    bool            haveFlags;       // 0x57
    bool            haveTwrManStr;   // 0x58
    bool            haveRefObj;      // 0x59
    // 0x5A..0x5B padding
    dsTSTRING<char> refObjName;      // 0x5C
    bool            haveDirToRefObj; // 0x60
    // 0x61..0x63 padding
    m3dV            dirToRefObj;     // 0x64
    bool            haveDirToHOPlane;// 0x70
    // 0x71..0x73 padding
    m3dV            dirToHOPlane;    // 0x74

    aiNAV_WP_DATA();
    ~aiNAV_WP_DATA();
    aiNAV_WP_DATA &operator=(const aiNAV_WP_DATA &rhs);

    // 0x8321A598 — copy `matrix` into `_mat` when present; returns haveMatrix.
    bool GetMatrix(m3dMATR &_mat) const;
};
