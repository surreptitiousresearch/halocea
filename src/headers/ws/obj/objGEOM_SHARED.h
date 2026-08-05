#pragma once
#include <stdint.h>
#include "../ap/apSTATE_T.h"
#include "../ds/dsARRAY.h"
#include "../m3d/m3dBOX.h"
// ws-engine obj: shared geometry payload for a model-object. DB-verified layout
// (types_members objGEOM_SHARED) — size 180 (0xB4).

struct objSPLIT;   // boundary — pointer only
struct objFACE;    // boundary — pointer only
struct m3dVTX;     // ../m3d/m3dVTX.h — pointer only
struct m3dV;       // ../m3d/m3dV.h  — pointer only
struct m4dV;       // ../m3d/m4dV.h  — pointer only

#ifndef objGEOM_SHARED_DEFINED
#define objGEOM_SHARED_DEFINED
typedef struct objGEOM_SHARED {
    apSTATE_T<int64_t> stateCompr;                    // 0x00 compression/format state bits
    uint64_t   fvf;                           // 0x08 flexible vertex format flags
    uint64_t   fvfInterleaved;                // 0x10
    uint64_t   fvfInstanced;                  // 0x18
    objSPLIT          *pSplit;                        // 0x20
    objFACE           *faceList;                      // 0x24
    m3dVTX            *texCoordList[5];               // 0x28
    unsigned int      *colorVertList[3];              // 0x3C
    m3dV              *vertList;                       // 0x48
    m3dV              *normalVertList;                 // 0x4C
    m4dV              *tangentList[5];                 // 0x50
    uint16_t   vertBoneIdxFirst;              // 0x64
    uint16_t   vertBoneIdxCount;              // 0x66
    uint8_t   *vertBoneIdxList;               // 0x68
    float             *vertBoneWeightList;            // 0x6C
    m3dBOX             bbox;                           // 0x70
    void              *pDataInst;                      // 0x88
    void              *pDataInterleaved;               // 0x8C
    ds::ARRAY<unsigned short, 5> nTexCoord;         // 0x90
    ds::ARRAY<unsigned short, 5> texCoordMaxTile;   // 0x9A
    ds::ARRAY<short, 3>          vertOffset;         // 0xA4
    ds::ARRAY<short, 3>          vertScale;          // 0xAA
    uint16_t   instNum;                        // 0xB0
    uint8_t    instSize;                       // 0xB2
    uint8_t    interleavedSize;                // 0xB3

    // Default ctor (used by objOBJ::_AllocGeom) and copy-construct a private clone of `src`
    // (used by objOBJ::_UnshareGeom). Bodies external to this batch.
    objGEOM_SHARED();                          // boundary
    objGEOM_SHARED(const objGEOM_SHARED &src); // boundary
} objGEOM_SHARED;
#endif
