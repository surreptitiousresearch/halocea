#pragma once
#include "../ap/apSTATE_T.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSTRID.h"
#include "../ds/WEAK_PTR.h"
#include "../ds/WEAK_PTR_BASE.h"
#include "../ds/MAP.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dOBB.h"
#include "../m3d/m3dMATR.h"
// phys subsystem physics-system object. Full DB-verified layout (types_members
// physSYSTEM_BASE, size 648) plus its by-value support records (physSYSTEM_BASE::sndFILTER,
// physIMP_ADV, safeAPPLY_FF, nested MOVING_STYLE enum). Method bodies beyond
// GetMatrMainBody are boundaries.

struct objOBJ;                 // ../obj/objOBJ.h — main-body model object (pointer only) boundary
struct animINST;               // ../anim/animINST.h — fallback anim instance (pointer only) boundary
struct entENTITY;              // ../ent/entENTITY.h (pointer only) boundary
struct physSYSTEM_BASE_SAFE;   // phys subsystem safe-handle target (pointer only)     boundary
struct physSYSTEM_BASE_vtbl;   // phys dispatch table (pointer only)                   boundary
struct physRB_BASE;            // phys rigid body (weak-ptr / pointer elements)        boundary
struct physBONE_IMP;           // phys per-bone impulse record (vector element)        boundary
struct physTIME_PARAMS;        // phys per-body time params (vector element)           boundary
struct physFORCE_SAVED;        // phys saved-force record (vector element)             boundary

// DB-verified layout (types_members physIMP_ADV) — size 20.
typedef struct physIMP_ADV {
    dsVECTOR<physBONE_IMP, 8> impSaved; // 0x00
} physIMP_ADV;

// DB-verified layout (types_members safeAPPLY_FF) — size 20.
typedef struct safeAPPLY_FF {
    int   isValid;         // 0x00
    float dt;              // 0x04
    m3dV  realSpeedApply;  // 0x08
} safeAPPLY_FF;

typedef struct physSYSTEM_BASE {
    // Nested DB types.
    struct sndFILTER {              // physSYSTEM_BASE::sndFILTER — size 12
        m3dV posMainBodyPrev;       // 0x00
    };
    enum MOVING_STYLE {             // physSYSTEM_BASE::MOVING_STYLE (DB-verbatim)
        MV_PHYSICS = 0, MV_ANIM_SET = 1, MV_ANIM_FOLLOW = 2, MV_NO_MOVING = 3,
    };
    // Creation descriptor — BOUNDARY partial. The full record is the next frontier; only the
    // leading state word (DB physSYSTEM_BASE::DESC::state@0) that aiPHYS_SMR::IsAcceptProcessing
    // reads (bit 10 = "participates in nav blocking") is modelled. Used by pointer only.
    struct DESC {
        apSTATE_T<unsigned long> state; // 0x00
    };

    physSYSTEM_BASE_vtbl              *__vftable;               // 0x000
    ds::WEAK_PTR_BASE<physSYSTEM_BASE> base;                    // 0x004 (anonymous base slot)
    void                              *m_pFollow;               // 0x008
    objOBJ                            *pObjMainBody;            // 0x00C main rigid body's model object, if any
    int                                indMainBody;             // 0x010
    m3dOBB                             obbMainBody;             // 0x014 (60B)
    float                              timeThrowByPlayer;       // 0x050
    dsVECTOR<ds::WEAK_PTR<physRB_BASE>, 8> bodiesHideByObb;     // 0x054
    dsVECTOR<ds::WEAK_PTR<physRB_BASE>, 8> bodiesHideByTimer;   // 0x068
    dsVECTOR<ds::WEAK_PTR<physRB_BASE>, 8> bodies2Remove;       // 0x07C
    int                                numberPartsActivated;    // 0x090
    struct sndFILTER                   sndFilter;               // 0x094 (12B)
    unsigned char                      _reserved : 7;           // 0x0A0 (DB anonymous 7-bit pad)
    unsigned char                      isClose2Cam : 1;         // 0x0A0 bit 7
    unsigned char                      _padA1[3];               // 0x0A1 db-verified padding
    animINST                          *pInst;                   // 0x0A4 anim instance fallback (used when no main body)
    apSTATE_T<unsigned long>           state;                   // 0x0A8
    float                              timeTerm;                // 0x0AC
    float                              swingForceX;             // 0x0B0
    float                              swingForceY;             // 0x0B4
    float                              timeNoAnyActorCollision; // 0x0B8
    MOVING_STYLE                       movingStyle;             // 0x0BC
    entENTITY                         *subEntity;               // 0x0C0
    const struct DESC                 *pDesc;                   // 0x0C4
    m3dV                               centerOBBStart;          // 0x0C8
    m3dOBB                             obbStart;                // 0x0D4 (60B)
    m3dMATR                            initInstMatr;            // 0x110 (64B)
    m3dMATR                            originalMatrL2W;         // 0x150 (64B)
    dsVECTOR<physRB_BASE *, 8>         rigidBodies;             // 0x190
    dsVECTOR<physTIME_PARAMS, 8>       timeParams;              // 0x1A4
    dsVECTOR<dsVECTOR<physFORCE_SAVED,8>, 8> forceSaved;        // 0x1B8
    dsVECTOR<dsSTRID, 8>               collisionExceptions;     // 0x1CC
    dsVECTOR<objOBJ *, 8>              obbWatches;              // 0x1E0
    dsVECTOR<objOBJ *, 8>              obbExtraToAdd;           // 0x1F4
    ds::MAP<objOBJ *, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> objCounters; // 0x208 (84B)
    physIMP_ADV                        impAdv;                  // 0x25C (20B)
    safeAPPLY_FF                       safeApplyFF;             // 0x270 (20B)
    physSYSTEM_BASE_SAFE              *pSystemSafe;             // 0x284 non-null while the system is alive

    // 0x82C40D80 — write the main rigid body's world transform into `out`; falls back to the
    // anim instance's local-to-world transform when there is no main body yet.
    void GetMatrMainBody(m3dMATR &out);
    // 0x82C44780 ?GetMainBody@physSYSTEM_BASE@@QAAPAVphysRB_BASE@@XZ — main rigid body (null if none).
    physRB_BASE *GetMainBody();
} physSYSTEM_BASE; // 648 bytes
