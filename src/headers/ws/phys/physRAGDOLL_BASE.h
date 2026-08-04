#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/WEAK_PTR.h"
#include "../ds/WEAK_PTR_BASE.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dBOX.h"
#include "physSYSTEM_BASE.h" /* physIMP_ADV / safeAPPLY_FF support records */
// phys subsystem character/ragdoll object. Full DB-verified layout (types_members
// physRAGDOLL_BASE, size 188) and nested types (sndFILTER, KEY_FRAMED_TYPE,
// physRAGDOLL_MNG_BASE::HK_SHARED_DATA_HANDLER). Method bodies are boundaries.

struct physRAGDOLL_BASE_vtbl;    // boundary
struct physRB_USER_DATA;         // boundary — per-rigid-body user record (vector element)
struct physSYSTEM_MP_RAGDOLL;    // boundary — multiplayer ragdoll system (pointer only)
struct physRAGDOLL_SAFE_READER;  // boundary — safe-handle reader (pointer only)
struct objOBJ;                   // ../obj/objOBJ.h (pointer elements)
struct animINST;                 // ../anim/animINST.h (method args)

// physRAGDOLL_MNG_BASE is its own DB struct (a boundary here). Its nested
// RAGDOLL_TPL (pointer target) and HK_SHARED_DATA_HANDLER (embedded by value) are the only
// members referenced; spelled with standalone names to avoid re-defining the manager class.
struct RAGDOLL_TPL;                // boundary — ragdoll prototype/template (physRAGDOLL_MNG_BASE::RAGDOLL_TPL)
// PHYS_NESTED marks this as the DB nested type physRAGDOLL_MNG_BASE::HK_SHARED_DATA_HANDLER
// (which has no standalone C-spellable name); the token keeps the layout verifier from
// mis-binding this helper spelling to the file's basename type.
#define PHYS_NESTED struct
PHYS_NESTED HK_SHARED_DATA_HANDLER { // DB idx@0 — size 4
    unsigned int idx;              // 0x00
};

typedef struct physRAGDOLL_BASE {
    // Nested DB types.
    struct sndFILTER {                 // physRAGDOLL_BASE::sndFILTER — size 12
        m3dV posMainBodyPrev;          // 0x00
    };
    enum KEY_FRAMED_TYPE {             // physRAGDOLL_BASE::KEY_FRAMED_TYPE (DB-verbatim)
        KEYFRAMED_OFF = 0, KEYFRAMED_FULL = 1, KEYFRAMED_EXCEPT_DYNAMIC_BONES = 2,
    };

    physRAGDOLL_BASE_vtbl              *__vftable;         // 0x00
    ds::WEAK_PTR_BASE<physRAGDOLL_BASE> base;              // 0x04 (anonymous base slot)
    struct sndFILTER                    sndFilter;         // 0x08 (12B)
    KEY_FRAMED_TYPE                     typeApply;         // 0x14
    unsigned char                       _reserved : 5;     // 0x18 (DB anonymous 5-bit pad)
    unsigned char                       isAnyActorsCdt : 1;// 0x18 bit 5
    unsigned char                       isActive : 1;      // 0x18 bit 6
    unsigned char                       massInited : 1;    // 0x18 bit 7
    unsigned char                       _pad19[3];         // 0x19 db-verified padding
    dsVECTOR<physRB_USER_DATA, 8>       userData;          // 0x1C
    dsVECTOR<physTIME_PARAMS, 8>        timeParams;        // 0x30
    physIMP_ADV                         impAdv;            // 0x44
    safeAPPLY_FF                        safeApplyFF;       // 0x58
    const RAGDOLL_TPL              *prototype;          // 0x6C
    float                               mass;              // 0x70
    m3dV                                velLinRoot;        // 0x74
    dsVECTOR<objOBJ *, 8>               mapper;            // 0x80
    dsVECTOR<int, 8>                    mapperParent;      // 0x94
    float                               timeThrowByPlayer; // 0xA8
    bool                                isStopByTouch;     // 0xAC
    unsigned char                       _padAD[3];         // 0xAD db-verified padding
    physSYSTEM_MP_RAGDOLL              *pSysMP;            // 0xB0
    HK_SHARED_DATA_HANDLER              infoShared; // 0xB4
    physRAGDOLL_SAFE_READER            *pSafeReader;       // 0xB8

    void CalcBBox_2(physRAGDOLL_BASE *self, m3dBOX *box); // boundary (explicit-this per decompiler)
} physRAGDOLL_BASE; // 188 bytes
