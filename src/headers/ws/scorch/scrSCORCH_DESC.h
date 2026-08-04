#pragma once
// ws-engine scorch/decal: the per-class scorch descriptor (ref-counted dsc product).
// DB-verified full layout (types_members scrSCORCH_DESC, size 172 / 0xAC); previously kept
// opaque in this corpus and read via raw offsets.
#include "../dsc/dscDESC.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h"

struct m3dSPL_LINEAR1D; // m3d 1D linear spline -- boundary (pointer only)
struct dsREF_COUNT;     // ../ds/dsREF_COUNT.h  -- boundary

// DB-verified (types_members scrSCALE_SPLINE) -- size 4.
typedef struct scrSCALE_SPLINE
{
    m3dSPL_LINEAR1D *pSpline; // 0x00
} scrSCALE_SPLINE;

// DB-verified (types_members scrSCORCH_DESC) -- offset-0 base is a base class in DB.
typedef struct scrSCORCH_DESC : dscDESC
{
    // DB nested type scrSCORCH_DESC::SCORCH_INI -- size 12.
    struct SCORCH_INI
    {
        float weight; // 0x00
        float radMin; // 0x04
        float radMax; // 0x08
    };

    float                            degRadStart;             // 0x0C
    float                            degRadEnd;               // 0x10
    float                            rotAngle;                // 0x14
    float                            rotAngleDisp;            // 0x18
    dsVECTOR<float, 8>               rotAngleValues;          // 0x1C (20)
    int                              isFilterByMaterial;      // 0x30
    int                              isFilterByDiscontinuity; // 0x34
    int                              isFilterByNormal;        // 0x38
    int                              isApplyColorFromLM;      // 0x3C
    int                              isKillGrass;             // 0x40
    float                            visibleAreaPart;         // 0x44
    int                              canBeOverlapped;         // 0x48
    float                            detailDensity;           // 0x4C
    float                            cloneOffset;             // 0x50
    scrSCALE_SPLINE                  splineScale;             // 0x54
    m3dSPL_LINEAR1D                 *decalColor;              // 0x58
    dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > spDefaultScorch; // 0x5C
    dsTSTRING<char>                  next;                    // 0x60
    int                              isGlowing;               // 0x64
    dsVECTOR<SCORCH_INI, 8>          cd;                      // 0x68 (20)
    dsSTRID                          classID;                 // 0x7C
    dsVECTOR<dsTSTRING<char>, 8>     textures;                // 0x80 (20)
    unsigned __int8                  blend;                   // 0x94
    unsigned char                    _pad95[3];               // 0x95 db-verified padding
    float                            radiusMin;               // 0x98
    float                            radiusMax;               // 0x9C
    float                            radNoAround;             // 0xA0
    float                            parallaxScale;           // 0xA4
    int                              isApplyTranspOnly;       // 0xA8
} scrSCORCH_DESC; // 172 bytes
