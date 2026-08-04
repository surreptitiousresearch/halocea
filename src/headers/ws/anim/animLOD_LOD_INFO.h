#pragma once
#include "../ds/dsVECTOR.h"
// anim subsystem: level-of-detail table for an animINST, plus one LOD record.
// DB-verified layout (types_members animLOD / animLOD::LOD_INFO): animLOD size 100 (0x64),
// LOD_INFO size 20.

class objOBJ;    // boundary — obj subsystem game object (vectors hold pointers)
struct animINST; // boundary — owning animated instance (pointer only)

typedef struct animLOD {
    struct LOD_INFO {
        dsVECTOR<objOBJ *, 8> objs; // 0x00 objects belonging to this LOD level

        // 0x82B0DAF8 — set bit 0 of every member object's stateProc: 0 when shown, 1 when hidden.
        void SetVisibility(bool show);
    };

    animINST                  *pInst;                 // 0x00 owning instance
    int                        stateLod;              // 0x04
    float                      distCamPrev;           // 0x08
    float                      distLODAnim;           // 0x0C
    float                      distLODOffMatrModel;   // 0x10
    float                      distLODPauseAnim;      // 0x14
    float                      distLODOffAnimBlend;   // 0x18
    float                      distLODOffLTAnimBlend; // 0x1C
    float                      distLODHideInst;       // 0x20
    float                      distLODHideObjInst;    // 0x24
    float                      distLODExtAnim;        // 0x28
    float                      transZone;             // 0x2C
    dsVECTOR<LOD_INFO, 8>      lods;                  // 0x30 per-level object vectors
    dsVECTOR<float, 8>         lodDist;               // 0x44 per-level switch distances
    int                        lodNmbCur;             // 0x58
    float                      lodFactorCur;          // 0x5C
    int                        lodNmbScripted;        // 0x60

    // 0x82B0D8C8 — construct the LOD table for `pInst`.  boundary (called by animINST::LODInit)
    animLOD(animINST *pInst);
    // 0x82B0E760 — build the per-level object buckets and switch distances from the instance model
    // and its ps "LOD" section.  reversed in ws_anim_0005.
    void Init();

private:
    // 0x82B0DFA0 — switch the active LOD level to `lodNmbNew` with blend factor `lodFactorNew`,
    // toggling per-level visibility. Returns true when the level/factor actually changed. (Private,
    // AAA; `lodFactorNew` is a float passed in an FPR — the decompiler widened it to double.)
    bool SetLodNmb(int lodNmbNew, float lodFactorNew);
} animLOD;
