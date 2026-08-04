#pragma once
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dMATR.h"
#include "../../apCL.h"
#include "navPATH.h"
#include "gphDEST.h"

// ws-engine nav subsystem — path-build query parameter block.
// DB-verified layout (types_members navQUERY) — size 212 (0xD4).

struct m3dMATR;
struct aiBRAIN;              // ai08                         boundary
struct dsGPH_QUERY;          // nav graph query state        boundary (ptr)
struct navMD_INDEX;          // nav mesh-descriptor index    boundary (opaque elems)
struct navSYS;               // nav                          boundary

struct navQUERY {
    short         wpidFrom;              // 0x00
    const m3dMATR *mFrom;                // 0x04
    aiBRAIN      *pBrain;                // 0x08
    m3dV         *vTo;                   // 0x0C
    short         plidTo;                // 0x10
    dsVECTOR<short, 8> toGlob;           // 0x14
    dsVECTOR<float, 8> toGlobLenMeters;  // 0x28
    float         minDist;               // 0x3C
    float         maxDist;               // 0x40
    bool          applyPenalties;        // 0x44
    bool          applyAdditionalLengthMeters; // 0x45
    bool          applyHOs;              // 0x46
    bool          applyUsedPaths;        // 0x47
    bool          isWideCheck;           // 0x48
    bool          isAssocCoinciding;     // 0x49
    unsigned char _pad4A[2];             // 0x4A
    float         overrideAssocDist;     // 0x4C
    unsigned short flagsDisabledEdge;    // 0x50
    unsigned char _pad52[2];             // 0x52
    unsigned int   flagsBlockedWP;       // 0x54
    unsigned int   mpID;                 // 0x58
    m3dMATR       matrFrom;              // 0x5C (64B)
    short          plidSrc;              // 0x9C
    unsigned char _pad9E[2];             // 0x9E
    dsGPH_QUERY   *dsQuery;              // 0xA0
    dsVECTOR<navMD_INDEX, 8> toLocal;    // 0xA4
    gphDEST        dest;                 // 0xB8 (28) graph-search destination descriptor

    // ctor(pBrain, apCL) / dtor defined elsewhere (nav).  boundary
    navQUERY(aiBRAIN *brain, const apCL *cl);
    ~navQUERY();
};
