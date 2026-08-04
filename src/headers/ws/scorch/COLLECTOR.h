#pragma once
#include "../cdt/cdtOFL.h"
#include "../cdt/cdtINFO.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"
// ws-engine scorch (gm_shared/scorch_poly_collector): collects the mesh faces a scorch-mark
// decal should be splattered onto for one collision hit. DB-verified layout
// (types_members scorch::COLLECTOR) — size 212 (0xD4).

struct objOBJ;         // ../obj/objOBJ.h                     boundary (pointer only)
struct objSPLIT_ENTRY; // ../obj subsystem                     boundary (pointer only)
struct atkDISPATCHER;  // attack-dispatch subsystem            boundary (pointer only)

namespace scorch {

// DB types_members scorch::OBJ_INFO — size 16 (13 + 3 trailing pad).
typedef struct OBJ_INFO {
    objOBJ           *pObj;      // 0x00
    objSPLIT_ENTRY   *pEntry;    // 0x04
    unsigned __int16  startFace; // 0x08
    unsigned __int16  nFace;     // 0x0A
    unsigned __int8   mtlID;     // 0x0C
} OBJ_INFO;

// DB types_members scorch::FILTER_STAT — size 16.
typedef struct FILTER_STAT {
    int facesBeforeFiltering;                  // 0x00
    int facesAfterInsidePolyTangFiltering;     // 0x04
    int facesAfterSurfaceFiltering;            // 0x08
    int facesAfterTangDiscontinuityFiltering;  // 0x0C
} FILTER_STAT;

typedef struct COLLECTOR {
    bool                            IsApplyTranspOnly;       // 0x00
    bool                            isShadeOnlySourceObj;    // 0x01
    bool                            isFilterByMaterial;       // 0x02 (not set by ctor — left as-is)
    bool                            isFilterByDiscontinuity;  // 0x03
    bool                            isFilterByNormal;         // 0x04
    unsigned char                   _pad05[3];                // 0x05
    m3dV                            normal;                   // 0x08 (not set by ctor)
    dsSTRID                         scorchName;               // 0x14
    const atkDISPATCHER            *pAtkDisp;                 // 0x18
    dsVECTOR<OBJ_INFO, 8>           objInfoList;               // 0x1C
    dsVECTOR<OBJ_INFO, 8>           objInfoListInst;           // 0x30
    dsVECTOR<unsigned short, 8>     faceIndList;                // 0x44
    dsVECTOR<unsigned short, 8>     faceIndListInst;            // 0x58
    dsVECTOR<unsigned short, 8>     faceIndListLM;               // 0x6C
    FILTER_STAT                     stat;                        // 0x80 (16B, not set by ctor)
    dsSTRID                         idNull;                       // 0x90
    cdtOFL                         *pOfl;                         // 0x94 owned, 500 primary / 0 secondary ids
    cdtOFL                         *pOflFace;                     // 0x98 owned, 1 primary / 3000 secondary ids
    objOBJ                         *pObjSource;                   // 0x9C
    m3dV                             pos;                          // 0xA0 (not set by ctor)
    cdtINFO                          cdtInfo;                      // 0xAC
    float                             radius;                       // 0xD0 (not set by ctor)

    // 0x82C913D4 — default-construct scorchName/idNull ("null")/cdtInfo, zero the five dsVECTOR
    // members, clear the three bool flags touched here (IsApplyTranspOnly/isShadeOnlySourceObj
    // false; isFilterByDiscontinuity/isFilterByNormal true), null pObjSource/pAtkDisp, then
    // heap-allocate pOfl (cdtOFL(500,0), single-level) and pOflFace (cdtOFL(1,3000)).
    // normal/pos/radius/stat/isFilterByMaterial are left untouched (matches the disassembly,
    // which never writes them here).
    COLLECTOR();

    // 0x82C9153C — delete pOfl/pOflFace (each via their own ~cdtOFL() + operator delete when
    // non-null), then free the five dsVECTOR members' backing storage directly (dlFree on
    // .pData; a plain-array teardown, not a per-element dtor walk — matches OBJ_INFO/unsigned
    // short both being trivially destructible).
    ~COLLECTOR();
} COLLECTOR;

} // namespace scorch
