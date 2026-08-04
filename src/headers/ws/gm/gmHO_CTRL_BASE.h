#pragma once
#include "../cdt/cdtHO_PARAMS.h"
#include "../ho/HO_POS.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m3dV.h"
#include "../gs/gsGEOM_ANCHOR.h"
// ws-engine gm: the runtime hideout controller — the live entered-HO state a body occupies.
// DB-verified (types_members gmHO_CTRL_BASE), size 0x184: __vftable@0, params@4 (cdtHO_PARAMS),
// matrSnap@0xE8 (m3dMATR), posCur@0x128 (HO_POS), vUpInitial@0x130, floorTran@0x13C,
// floorInfo@0x148 (gsGEOM_ANCHOR, 60B — ends 0x184).

struct m3dTR;
struct gmHO_CTRL_BASE_vtbl;

struct gmHO_CTRL_BASE {
    gmHO_CTRL_BASE_vtbl *__vftable;      // 0x00
    cdtHO_PARAMS params;                 // 0x04 (228 bytes -> ends 0xE8)
    m3dMATR       matrSnap;              // 0xE8
    HO_POS        posCur;                // 0x128
    m3dV          vUpInitial;            // 0x130
    m3dV          floorTran;             // 0x13C
    gsGEOM_ANCHOR floorInfo;             // 0x148

    // Current body world position on this HO. Body external (gm subsystem).
    const m3dV *GetBodyPos(); // boundary
    // Constrain a requested movement transform against this HO's geometry. Body external.
    void ConstraintMovement(const m3dTR *mv); // boundary
    // Deleting-destructor / refcounted release (vtbl scalar-deleting dtr, flag 1). Body external.
    void Release(); // boundary
};
