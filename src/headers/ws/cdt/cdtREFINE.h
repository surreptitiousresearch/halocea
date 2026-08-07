#pragma once
// ws-engine cdt (collision-detection): a collision-query "refinement" filter object. It carries
// the inclusion/exclusion state used to accept or reject candidate contacts during a query
// (self instance to skip, layer mask, actor/object filter flags), and is weak-referenceable so a
// pending async query can drop it safely. DB-verified layout (types_members cdtREFINE, size 32):
//   __vftable@0, ds::WEAK_PTR_BASE<cdtREFINE>@4 (pHandle), stateObjUsrIncl@8 (apSTATE_T<__int64>),
//   pInstSelf@16 (animINST*), state@20 (apSTATE_T<unsigned long>), layerMask@24, specMode@28.

#include <stdint.h>
#include "../ds/WEAK_PTR_BASE.h"
#include "../ap/apSTATE_T.h"

struct animINST;      // ../anim/animINST.h — pointer only
struct objOBJ;        // boundary — vtbl param only
struct cdtREFINE;

// DB-verified vtbl (types_members cdtREFINE_vtbl).
typedef struct cdtREFINE_vtbl {
    void (*dtr_cdtREFINE)(cdtREFINE *self, int deleteFlag);                       // 0x00 — deleting dtor: vftable+0x00 holds ??_GcdtREFINE@@UAAPAXI@Z
    int  (*IsInclInst)(cdtREFINE *self, animINST *inst);          // 0x04
    void (*Modify)(cdtREFINE *self);                              // 0x08
    int  (*IsInclObjActors)(cdtREFINE *self, objOBJ *obj);        // 0x0C
    int  (*IsInclObjStatDBG)(cdtREFINE *self, objOBJ *obj);       // 0x10
} cdtREFINE_vtbl;

typedef struct cdtREFINE {
    enum SELDOM {
        CENTROIDS_ONLY        = 0,
        CENTERS_ONLY          = 1,
        CENTROIDS_AND_CENTERS = 2,
        UNDEF                 = 3,
    };

    cdtREFINE_vtbl              *__vftable;    // 0x00
    ds::WEAK_PTR_BASE<cdtREFINE> weakSelf;     // 0x04 (pHandle)
    apSTATE_T<int64_t>       stateObjUsrIncl; // 0x08 user object-inclusion state (low) + layer (high)
    animINST                *pInstSelf;       // 0x10 the querying instance, excluded from results
    apSTATE_T<unsigned long> state;           // 0x14 refine flags
    unsigned int             layerMask;       // 0x18 collision layer mask
    SELDOM                   specMode;        // 0x1C sparse-sampling mode (default = 5, see ctor)

    // 0x827139E8 — construct from (flags, layerMask, user object-inclusion state, self inst).
    // DEVIATION (2026-08-07): this was modeled with THREE parameters, folding layerMask out of
    // the high dword of stateObjUsrIncl. It takes FOUR. The mangle is
    // ??0cdtREFINE@@QAA@HKV?$apSTATE_T@_J@@PAVanimINST@@@Z = (int, unsigned long,
    // apSTATE_T<__int64>, animINST *), and the prologue stores confirm it register-for-register:
    // stw r4,0x14 (state=flags) / stw r5,0x18 (layerMask) / std r6,8 (the whole 64-bit
    // apSTATE_T, one GPR — the 360's GPRs are 64-bit) / stw r7,0x10 (pInstSelf). The 3-arg model
    // computed layerMask = HIDWORD(stateObjUsrIncl), which is a different value at every call
    // site that passes them independently (hcex_create_decals_delayed @0x823DDF3C passes
    // layerMask=0 in r5 while r6 holds 0xC004000FBF, whose high dword is 0xC0).
    cdtREFINE(int flags, unsigned long layerMask, apSTATE_T<int64_t> stateObjUsrIncl,
              animINST *pInstSelf);
    // 0x823D6590 — release weak handle on destroy. Dispatched through the explicit __vftable
    // member (slot 0); declared non-virtual so MSVC emits no implicit vptr — a `virtual` here
    // would shift every member +4 vs the DB layout (single-vptr convention, as in cdtBONE et al).
    ~cdtREFINE();
} cdtREFINE;
