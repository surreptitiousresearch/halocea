#pragma once
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
// ws-engine cdt: a single collision bone within a cdtSKEL's bone list.
// DB-verified layout (types_members cdtBONE) — size 40 (0x28).

struct objOBJ;   // ../obj/objOBJ.h — owning geometry object (pointer only)   boundary
struct animINST; // ../anim/animINST.h — instance context for queries         boundary
struct m3dBOX;   // ../m3d/m3dBOX.h                                            boundary

struct cdtBONE_vtbl; // boundary — cdt subsystem vtable

typedef struct cdtBONE {
    cdtBONE_vtbl                  *__vftable;  // 0x00
    dsSTRID                        boneID;     // 0x04
    int                             ragBone;    // 0x08
    const cdtBONE                 *parent;     // 0x0C
    dsVECTOR<const cdtBONE *, 8>   childs;      // 0x10
    int                             uid;        // 0x24

    // Resolve the objOBJ instance this bone maps to within `pInst`. boundary — not decompiled here.
    objOBJ *GetObjFrom(animINST *pInst); // boundary

    // Virtual (cdtBONE_vtbl+0xC) — the bone's world-space centre within `pInst` (by value).
    // Reached by CalcYHeightFromMatrToBone (aiWATCHER::UpdateHeights). boundary (vtbl slot 3).
    struct m3dV GetCenter(animINST *pInst) const; // boundary

    // Grow `pBox` to enclose this bone's geometry within `pInst`. boundary — not decompiled here.
    void GetEnclBox(animINST *pInst, m3dBOX *pBox); // boundary

    // Closest-point query of `point` (within `radius`) against this bone's collision volume in
    // `pInst`; `pRefine` steers sub-bone refinement, the hit is written into `pOut`. Returns
    // nonzero on a hit. Dispatched at the call site through __vftable slot 6 (offset +24).
    // (The `radius` float shadows the r6 GPR slot, which is why the decompiler shows a spurious
    // int param there.) boundary — not decompiled here.
    int DistPoint(animINST *pInst, const struct m3dV *point, float radius,
                  struct cdtREFINE *pRefine, struct cdtINFO *pOut); // boundary (vtbl slot 6)
} cdtBONE;
