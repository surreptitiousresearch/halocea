#pragma once
#include "../ds/dsVECTOR.h"
// ws-engine nav (graph) — path-build destination descriptor handed to the graph search.
// DB-verified layout (types_members gphDEST): gph@0, vTo@4, idTo@8, wi@12, minDist@16,
// maxDist@20, mpID@24 — size 28.

struct m3dV;
struct navMD_INDEX; // nav mesh-descriptor index — boundary (opaque elems)

struct gphDEST {
    // DEVIATION: DB type is `navGPH *`, but the corpus models navGPH as a *namespace*
    // (navGPH_boundary.h — navGPH::navBLOCK_OBB), so a `struct navGPH` fwd-decl would clash in
    // TUs that see both. Kept as void* at the exact offset/width.
    void                     *gph;     // 0x00 owning nav graph (DB: navGPH *)
    m3dV                     *vTo;     // 0x04 destination point
    short                     idTo;    // 0x08 destination waypoint id
    dsVECTOR<navMD_INDEX, 8> *wi;      // 0x0C candidate local indices
    float                     minDist; // 0x10
    float                     maxDist; // 0x14
    unsigned int              mpID;    // 0x18
};
