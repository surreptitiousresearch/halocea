#pragma once
#include "cdtBONE.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dBOX.h"
// ws-engine cdt: per-anim-instance skeleton collision cache — bone list plus a
// list of additional (non-skeletal) collidable objects.
// DB-verified layout (types_members cdtSKEL) — size 44 (0x2C).

struct animINST; // ../anim/animINST.h — instance being queried   boundary
struct m3dV;     // ../m3d/m3dV.h — query point (pointer only)
struct cdtINFO;  // cdtINFO.h — hit-result record (pointer only)
struct cdtREFINE; // cdt sub-bone refinement steering; opaque here (pointer only)   boundary

typedef struct cdtSKEL {
    dsVECTOR<short, 8>       listAdditionalObjects; // 0x00
    dsVECTOR<cdtBONE *, 8>   bones;                  // 0x14
    int                       uniqueRagBoneIDCount;   // 0x28

    // 0x82B0C538 (?FindBoneByName@cdtSKEL@@QBAPBVcdtBONE@@ABVdsSTRID@@@Z) — the bone whose boneID
    // matches `name` (null if none). const. boundary — body external to this batch.
    const cdtBONE *FindBoneByName(const dsSTRID &name) const;

    // 0x82B0B818 — recompute local-to-model matrices for `pInst`. Body not decompiled here
    // (thin wrapper; see CalcLTMExcl for the exclusive-lock variant).
    void CalcLTM(animINST *pInst);
    // 0x82B0B810 — exclusive-locked variant of CalcLTM.
    void CalcLTMExcl(animINST *pInst);

    // 0x82B0B5E8 — apply the current animation pose to `pInst`.
    void CalcAnim(animINST *pInst);
    // 0x82B0B5D0 — exclusive-locked variant of CalcAnim.
    void CalcAnimExcl(animINST *pInst);

    // 0x82B0C2C0 — compute the enclosing box over all visible bones of `pInst` (or fall back
    // to the instance's own bounding box when this skeleton has no bones).
    void GetEnclBox(animINST *pInst, m3dBOX *pBox);

    // 0x82B0BEC4 — closest-point query of `point` (within `radius`) against the instance's
    // per-bone collision volumes. Broad-phase rejects via the instance OBB, then tests each
    // bone, keeping the nearest hit in `pInfo`; `pRefine` steers sub-bone refinement. Returns
    // nonzero on a hit. const. REVERSED: src/ws/cdt/cdtSKEL__DistPoint.cpp.
    int DistPoint(animINST *pInst, const m3dV *point, float radius,
                  cdtREFINE *pRefine, cdtINFO *pInfo) const;

    // Draw the skeleton's per-bone collision volumes for `pInst` (debug visualisation, called from
    // aiDEBUG::RenderCDTGroup). Body belongs to the cdt subsystem — boundary.
    void Render(animINST *pInst);
} cdtSKEL;
