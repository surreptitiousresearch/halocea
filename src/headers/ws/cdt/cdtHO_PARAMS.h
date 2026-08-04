#pragma once
#include "../m3d/m3dV.h"
#include "../ap/apSTATE_T.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"
// ws-engine cdt: hideout (HO) enter/query parameter blocks. DB-verified layouts
// (types_members cdtHO_ENTER_PARAMS size 0x9C, cdtHO_PARAMS size 0xE4). Bodies of the member
// functions are external (cdt subsystem / next frontier); only decls are provided here.

struct HO_POS;         // ho — edge/pos pair          boundary (ptr)
struct objOBJ;         // obj — game object           boundary (ptr)
struct hoOBJECT;       // ho — hideout object         boundary (ptr)
struct entENTITY;      // ent — entity                boundary (ptr)
struct srQUERY_SRAY;   // sr — swept-ray query        boundary (ptr)
struct srQUERY_SCYL;   // sr — swept-cyl query        boundary (ptr)
struct cdtHO_REFINE;   // cdt — HO refine params      boundary (ptr)

// DB-verified (types_members cdtHO_ENTER_PARAMS), size 0x9C.
struct cdtHO_ENTER_PARAMS {
    apSTATE_T<unsigned short> stateIn;    // 0x00
    float          hoEnterDist;           // 0x04
    float          angleDivergence;       // 0x08
    float          maxDirDiff;            // 0x0C
    unsigned char  hoEnterType;           // 0x10
    HO_POS        *pHoPosCur;             // 0x14
    objOBJ        *pObjHolderIgnore;      // 0x18
    objOBJ        *pObjHolderIgnore1;     // 0x1C
    hoOBJECT      *pHideoutExclude;       // 0x20
    entENTITY     *pEntTarget;            // 0x24
    objOBJ        *pObjTarget;            // 0x28
    bool           isStaticOnly;          // 0x2C
    entENTITY     *pEntUser;              // 0x30
    srQUERY_SRAY  *pQuerySRay;            // 0x34
    srQUERY_SCYL  *pQuerySCyl;            // 0x38
    cdtHO_REFINE  *pRefineSRay;           // 0x3C
    cdtHO_REFINE  *pRefineSCyl;           // 0x40
    dsTSTRING<char> dbgInfo;              // 0x44
    m3dV           vUp;                   // 0x48
    m3dV           vGravUp;               // 0x54
    m3dV           vCamUp;                // 0x60
    m3dV           vForw;                 // 0x6C
    m3dV           vCamForw;              // 0x78
    m3dV           vOrg;                  // 0x84
    m3dV           vCamOrg;               // 0x90

    // Initialise the world-space enter frame from origin / forward / up and a hoEnterType code.
    void Init(const m3dV &vOrg, const m3dV &vForw, const m3dV &vUp, unsigned hoEnterType); // boundary
    // Record the source waypoint's parent entity and name into dbgInfo/target fields.
    void SetWpInfo(entENTITY *parent, dsTSTRING<char> *name); // boundary
};

struct cdtHO_PARAMS_vtbl;

// DB-verified (types_members cdtHO_PARAMS), size 0xE4.
struct cdtHO_PARAMS {
    cdtHO_PARAMS_vtbl *__vftable;         // 0x00
    cdtHO_ENTER_PARAMS enterParams;       // 0x04 (unnamed cdtHO_ENTER_PARAMS in DB)
    float   cdtRadius;                    // 0xA0
    float   cdtRadiusHigh;                // 0xA4
    float   cdtHeight;                    // 0xA8
    float   egdeOffsetLeft;               // 0xAC
    float   egdeOffsetRight;              // 0xB0
    float   egdeOffsetLeftHigh;           // 0xB4
    float   egdeOffsetRightHigh;          // 0xB8
    apSTATE_T<unsigned char> stateOut;    // 0xBC
    m3dV    dirSnap;                       // 0xC0
    float   egdeOffsetUp;                 // 0xCC
    float   centerBodyOffset;             // 0xD0
    float   leftBodyOffset;               // 0xD4
    float   rightBodyOffset;              // 0xD8
    bool    meleeUseLeftPoint;            // 0xDC
    bool    meleeUseRightPoint;           // 0xDD
    bool    meleeUseCenterPoint;          // 0xDE
    dsSTRID paramsType;                   // 0xE0

    cdtHO_PARAMS();                        // boundary
    ~cdtHO_PARAMS();                       // boundary
    cdtHO_PARAMS &operator=(const cdtHO_PARAMS &other); // boundary
};
