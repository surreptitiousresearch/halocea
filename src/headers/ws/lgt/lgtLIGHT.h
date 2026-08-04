#pragma once
#include "../m3d/m3dMATR.h"
#include "../m3d/m3dCOLOR.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dOBB.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsFLAGS.h"
#include "lgtID.h"
// ws-engine lgt: a runtime light instance. DB-verified layout (types_members lgtLIGHT) — size
// 240 (0xF0). The sel/ps/affix/logic sub-objects that the reversed light methods never touch
// are kept as raw padding at their exact DB offset/size so sizeof() and downstream offsets stay
// correct without pulling in those subsystems; every scalar light parameter read by
// CalcAttenuation / GetOBB / rendLIGHT_SET::UpdateStatic is given its concrete DB type.

enum LIGHT_STATE : int;   // lgt subsystem flag enum — boundary (only bit 0 read here)

// lgtID now carries its DB-verified { typeId, id } layout — see lgtID.h.

struct m3dV; // fwd (via m3dV.h)

typedef struct lgtLIGHT {
    dsFLAGS<LIGHT_STATE, int> state;       // 0x00  bit 0 tested (light enabled)
    unsigned char             _pad04[24];  // 0x04  affixes(4)+objSel(16)+ps(4)  boundary
    lgtID                     id;          // 0x1C  passed to rendSR_DATA::IsLampVisible
    dsTSTRING<char>           name;        // 0x20
    int                       type;        // 0x24  0=omni,1=spot,2=pure-ambient,3=linear,4=cube
    int                       atten;       // 0x28  attenuation model selector (0=none,2..4 curves)
    m3dMATR                   matrL2W;     // 0x2C (64)  local-to-world; v[3]/elements[12..14]=origin
    m3dCOLOR                  colorD;      // 0x6C (16)  diffuse RGBA
    float                     colorMultiplier;      // 0x7C
    float                     intensity;            // 0x80
    float                     attenStart;           // 0x84
    float                     attenEnd;             // 0x88
    m3dV                      cubeAttenStartPos;    // 0x8C (12)
    m3dV                      cubeAttenStartNeg;    // 0x98 (12)
    m3dV                      cubeAttenEnd;         // 0xA4 (12)
    float                     hotSpotAngle;         // 0xB0
    float                     fallOffAngle;         // 0xB4
    float                     scale;                // 0xB8
    float                     scaleDeg;             // 0xBC
    float                     nearPlane;            // 0xC0
    int                       smResolution;         // 0xC4
    float                     pssmDist;             // 0xC8
    int                       numSplits;            // 0xCC
    float                     filterRadiusScale;    // 0xD0
    float                     resolutionReduceSpeed;// 0xD4
    float                     distVisFactor;        // 0xD8
    bool                      excludeGrass;         // 0xDC
    bool                      disableSMData;        // 0xDD
    unsigned char             _padDE[2];            // 0xDE  alignment
    unsigned char             _padLogicParams[8];   // 0xE0  logicParams (lgtLOGIC_PARAMS)  boundary
    float                     colorMultiplier2;     // 0xE8
    float                     radius2;              // 0xEC
    // total size 0xF0 (240)

    // 0x82A7A1EC — evaluate this light's attenuation at world position `point` (0 = no
    // contribution, 1 = full). REVERSED: src/ws/lgt/lgtLIGHT__CalcAttenuation.cpp.
    float CalcAttenuation(m3dV *point) const;

    // 0x82A79E5C — compute the light's bounding OBB into `*pOBB`; returns 0 when the light has
    // no attenuation volume, else 1. REVERSED: src/ws/lgt/lgtLIGHT__GetOBB.cpp.
    int GetOBB(m3dOBB *pOBB);

    // lgt-subsystem statics used by CalcAttenuation — bodies not in this batch  boundary
    static void GetLightDistAttenCoeffs(float *outA, float *outB, float *outC,
                                        float attenEnd, float attenStart, bool flag); // boundary
} lgtLIGHT;

// 0x82A7A340 — per-axis cube-light attenuation (?lgtLIGHT__CalcCubeAttenuation@@YAMMMMM@Z),
// a free function despite the lgtLIGHT__ prefix.  boundary
float lgtLIGHT__CalcCubeAttenuation(float coord, float attenEnd, float startPos, float startNeg);
