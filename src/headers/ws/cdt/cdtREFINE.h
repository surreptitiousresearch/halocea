#pragma once
// ws-engine cdt (collision-detection): a collision-query "refinement" filter object. It carries
// the inclusion/exclusion state used to accept or reject candidate contacts during a query
// (self instance to skip, layer mask, actor/object filter flags), and is weak-referenceable so a
// pending async query can drop it safely. DB-verified layout (types_members cdtREFINE, size 32):
//   __vftable@0, ds::WEAK_PTR_BASE<cdtREFINE>@4 (pHandle), stateObjUsrIncl@8 (apSTATE_T<__int64>),
//   pInstSelf@16 (animINST*), state@20 (apSTATE_T<unsigned long>), layerMask@24, specMode@28.

#include "../ds/WEAK_PTR_BASE.h"
#include "../ap/apSTATE_T.h"

struct animINST;      // ../anim/animINST.h — pointer only
struct objOBJ;        // boundary — vtbl param only
struct cdtREFINE;

// DB-verified vtbl (types_members cdtREFINE_vtbl).
typedef struct cdtREFINE_vtbl {
    void (*dtr_cdtREFINE)(cdtREFINE *self);                       // 0x00
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
    apSTATE_T<__int64>       stateObjUsrIncl; // 0x08 user object-inclusion state (low) + layer (high)
    animINST                *pInstSelf;       // 0x10 the querying instance, excluded from results
    apSTATE_T<unsigned long> state;           // 0x14 refine flags
    unsigned int             layerMask;       // 0x18 collision layer mask
    SELDOM                   specMode;        // 0x1C sparse-sampling mode (default = 5, see ctor)

    // 0x827139E8 — construct from (flags, layerMask+objState packed in _stateObjUsrIncl, self inst).
    cdtREFINE(unsigned int flags, __int64 stateObjUsrIncl, animINST *pInstSelf);
    // 0x823D6590 — release weak handle on destroy. Dispatched through the explicit __vftable
    // member (slot 0); declared non-virtual so MSVC emits no implicit vptr — a `virtual` here
    // would shift every member +4 vs the DB layout (single-vptr convention, as in cdtBONE et al).
    ~cdtREFINE();
} cdtREFINE;
