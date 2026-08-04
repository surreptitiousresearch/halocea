#pragma once
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dVTX.h"
#include "objGEOM_SHARED.h"
// ws-engine obj: per-object (unshared) geometry wrapper around an objGEOM_SHARED.
// DB-verified layout (types_members objGEOM_UNSHARED) — size 71 (0x47).

struct objOBJ;       // boundary — pointer only
struct rendORD;      // boundary — pointer only
struct objMODIFIER;  // boundary — pointer only

#ifndef objGEOM_UNSHARED_DEFINED
#define objGEOM_UNSHARED_DEFINED
typedef struct objGEOM_UNSHARED {
    apSTATE_T<__int64> stateDyn;             // 0x00
    objOBJ            *pSharingHostObj;       // 0x08
    objGEOM_SHARED    *pSharedGeom;           // 0x0C
    int                sharingOfsVtx;         // 0x10
    int                sharingOfsFace;        // 0x14
    rendORD           *pRendData;             // 0x18
    unsigned int       color;                 // 0x1C
    unsigned int       color_mp;              // 0x20
    m3dVTX             texOffset[2];          // 0x24
    objMODIFIER      **modList;               // 0x34
    float              transp;                // 0x38
    float              lodTransp;             // 0x3C
    __int16            nModifiers;            // 0x40
    unsigned __int8    alphaKillValue;        // 0x42
    unsigned __int8    zBiasValue;            // 0x43
    char               renderPassId;          // 0x44
    unsigned __int8    blendRend;             // 0x45
    unsigned __int8    transpRendPriority;    // 0x46
} objGEOM_UNSHARED;
#endif
