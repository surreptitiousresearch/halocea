#pragma once
#include <stdint.h>
#include "../ap/apSTATE_T.h"
#include "../ds/dsAFFIX_STRING.h"
#include "../ds/dsARRAY.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m3dVTX.h"
#include "../m3d/m3dBOX.h"
#include "../m3d/m3dOBB.h"
#include "../mtl/mtlTBL.h"
#include "../ps/psSECTION.h"
// anim-local obj-subsystem BOUNDARY graph. The real ws/obj/objOBJ.h re-includes a conflicting
// legacy-Blam animINST.h, so the anim validation/LOD/render-list methods model the obj graph they
// touch here instead. All layouts are DB-verified full bodies (types_members objOBJ /
// objGEOM_UNSHARED / objGEOM_SHARED / objSPLIT / objSPLIT_ENTRY / objBONES_INFO).

struct animINST;         // animINST.h — owning instance (pointer only)  boundary
struct objMODIFIER;      // obj subsystem modifier (pointer only)        boundary
struct objSPU_CONFIG;    // boundary — pointer only
struct objFACE;          // boundary — pointer only
struct m3dVTX;           // ../m3d/m3dVTX.h — pointer only
struct m3dV;             // ../m3d/m3dV.h  — pointer only
struct m4dV;             // ../m3d/m4dV.h  — pointer only
struct rendORD;          // rend subsystem render-order data (pointer only) boundary
struct objOBJ;           // defined below (self/geometry back-pointers)

// Per-split bone range info. DB-verified layout (types_members objBONES_INFO) — size 6.
typedef struct objBONES_INFO {
    uint16_t start1; // 0x00
    uint16_t start2; // 0x02
    uint8_t  num1;   // 0x04
    uint8_t  num2;   // 0x05
} objBONES_INFO;

// Per-material draw split entry — DB objSPLIT_ENTRY size 92 (0x5C).
typedef struct objSPLIT_ENTRY {
    mtlTBL           mtlTbl;         // 0x00 materials for this batch
    uint16_t __dummy__;      // 0x3C
    uint16_t startVert;      // 0x3E
    uint16_t nVert;          // 0x40
    uint16_t startFace;      // 0x42
    uint16_t nFace;          // 0x44
    unsigned char    _pad46[2];      // 0x46 db-verified padding
    objSPU_CONFIG   *spuConfig;      // 0x48
    objBONES_INFO    bonesInfo;      // 0x4C
    int16_t          skinCompoundId; // 0x52 skin-compound object id
    int              maxVSLength;    // 0x54
    int              maxPSLength;    // 0x58
} objSPLIT_ENTRY;

// Draw-split table — DB objSPLIT size 8.
typedef struct objSPLIT {
    unsigned short  nEntry;         // 0x00 number of split entries
    unsigned char   _pad2[2];       // 0x02
    objSPLIT_ENTRY *entryList;      // 0x04

    objSPLIT_ENTRY *GetEntry(int entryNmb); // 0x8265F2A8  boundary
} objSPLIT;

// Shared geometry block — DB objGEOM_SHARED size 180 (0xB4).
// Guarded so a TU that also pulls the canonical ws/obj/objGEOM_SHARED.h (identical DB layout) does
// not double-define (C2011); whichever header is included first wins.
#ifndef objGEOM_SHARED_DEFINED
#define objGEOM_SHARED_DEFINED
typedef struct objGEOM_SHARED {
    apSTATE_T<int64_t> stateCompr;                  // 0x00 compression/format state bits
    uint64_t   fvf;                         // 0x08 flexible vertex format flags
    uint64_t   fvfInterleaved;              // 0x10
    uint64_t   fvfInstanced;                // 0x18
    objSPLIT          *pSplit;                      // 0x20 draw-split table
    objFACE           *faceList;                    // 0x24
    m3dVTX            *texCoordList[5];             // 0x28
    unsigned int      *colorVertList[3];            // 0x3C
    m3dV              *vertList;                    // 0x48
    m3dV              *normalVertList;              // 0x4C
    m4dV              *tangentList[5];              // 0x50
    uint16_t   vertBoneIdxFirst;            // 0x64 first skin-compound object index
    uint16_t   vertBoneIdxCount;            // 0x66 skin-compound object count
    uint8_t   *vertBoneIdxList;             // 0x68
    float             *vertBoneWeightList;          // 0x6C
    m3dBOX             bbox;                        // 0x70
    void              *pDataInst;                   // 0x88
    void              *pDataInterleaved;            // 0x8C
    ds::ARRAY<unsigned short, 5> nTexCoord;         // 0x90
    ds::ARRAY<unsigned short, 5> texCoordMaxTile;   // 0x9A
    ds::ARRAY<short, 3>          vertOffset;        // 0xA4
    ds::ARRAY<short, 3>          vertScale;         // 0xAA
    uint16_t   instNum;                     // 0xB0
    uint8_t    instSize;                    // 0xB2
    uint8_t    interleavedSize;             // 0xB3
} objGEOM_SHARED;
#endif

// Per-object unshared geometry block — DB objGEOM_UNSHARED size 72 (0x48).
#ifndef objGEOM_UNSHARED_DEFINED
#define objGEOM_UNSHARED_DEFINED
typedef struct objGEOM_UNSHARED {
    apSTATE_T<int64_t> stateDyn;           // 0x00
    objOBJ            *pSharingHostObj;    // 0x08
    objGEOM_SHARED    *pSharedGeom;        // 0x0C shared geometry
    int                sharingOfsVtx;      // 0x10
    int                sharingOfsFace;     // 0x14
    rendORD           *pRendData;          // 0x18
    unsigned int       color;              // 0x1C
    unsigned int       color_mp;           // 0x20
    m3dVTX             texOffset[2];       // 0x24
    objMODIFIER      **modList;            // 0x34
    float              transp;             // 0x38
    float              lodTransp;          // 0x3C
    int16_t            nModifiers;         // 0x40 number of modifiers
    uint8_t    alphaKillValue;     // 0x42
    uint8_t    zBiasValue;         // 0x43
    char               renderPassId;       // 0x44
    uint8_t    blendRend;          // 0x45
    uint8_t    transpRendPriority; // 0x46
    unsigned char      _pad47[1];          // 0x47 db-verified tail padding
} objGEOM_UNSHARED;
#endif

// obj-subsystem model object — DB objOBJ size 220 (0xDC).
#ifndef objOBJ_DEFINED
#define objOBJ_DEFINED
typedef struct objOBJ {
    char                    *name;             // 0x00
    int16_t                  id;               // 0x04
    unsigned char            _pad6[2];         // 0x06
    objGEOM_UNSHARED        *pGeom;            // 0x08
    int                      nFace;            // 0x0C
    int                      nVert;            // 0x10
    animINST                *pInst;            // 0x14
    int                      state;            // 0x18
    int                      stateProc;        // 0x1C
    apSTATE_T<int64_t>       stateUsr64;       // 0x20
    apSTATE_T<int64_t>       stateShare;       // 0x28
    apSTATE_T<unsigned char> stateVis;         // 0x30
    unsigned char            _pad31[3];        // 0x31 db-verified padding
    dsAFFIX_STRING           affixes;          // 0x34
    unsigned int             lastFameVisible;  // 0x38
    objOBJ                  *parent;           // 0x3C
    m3dMATR                  matrLT;           // 0x40
    objOBJ                  *next;             // 0x80
    objOBJ                  *prev;             // 0x84
    objOBJ                  *child;            // 0x88
    m3dMATR                  matrModel;        // 0x8C model transform (matrLT * parent^-1)
    m3dMATR                 *pMatrLTPrev;      // 0xCC
    psSECTION                ps;               // 0xD0
    int16_t                  animNmb;          // 0xD4
    uint16_t         stateRend;        // 0xD6
    int                      dbgRenderRefCount;// 0xD8

    static void Delete(objOBJ *obj);          // 0x82A7EB40  boundary
    void RemoveModifiers();                    // 0x82A8AD28  boundary
    int  ApplyModifiers(int stateVldMask);     // 0x82A8AE30  boundary
    void    GetVert(int idx, m3dV *pResult);   // 0x82BCE098  boundary — vertex position by index
    void    GetOBB(m3dOBB *obb);               // 0x82A719D8  boundary — object-space OBB
    objFACE *GetFace(int idx);                 // 0x82661278  boundary — face record by index
} objOBJ;
#endif
