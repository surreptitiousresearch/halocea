#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dMATR.h"
#include "instCONSTRUCTOR_NODE_RELATIONS.h"
// ws-engine prop subsystem: one node of the prop-instance construction tree used by
// instCONSTRUCTOR_MNG to build a prop instance from named preset/skin presets.
// DB-verified layout (types_members instCONSTRUCTOR_NODE):
//   objName@0 (dsTSTRING<char>), objId@4 (int), exclusive@8 (bool), parent@12
//   (instCONSTRUCTOR_NODE*), childs@16 (dsVECTOR<instCONSTRUCTOR_NODE*,8>), prob@36 (int),
//   childProbSum@40 (int), transofrm@44 (m3dMATR, 64B) — size 108.
// `transofrm` is the verbatim (misspelled) DB member name — kept as-is per naming determinism.

struct animINST;   // ../anim/animINST.h — instance being constructed (pointer only)  boundary

typedef struct instCONSTRUCTOR_NODE {
    dsTSTRING<char>        objName;       // 0x00 preset/skin name (GetPresetName returns this by reference)
    int                    objId;         // 0x04
    bool                   exclusive;     // 0x08
    unsigned char          _pad09[3];     // 0x09
    instCONSTRUCTOR_NODE  *parent;        // 0x0C
    dsVECTOR<instCONSTRUCTOR_NODE *, 8> childs; // 0x10
    int                    prob;          // 0x24
    int                    childProbSum;  // 0x28
    m3dMATR                transofrm;     // 0x2C

    // 0x826E1FA4 — apply a named RELATION override (looked up by ancestor/child object name via
    // `rels->GetRelation`) between this node's object and its nearest ancestor that has a valid
    // `objId`: recompute this node's `transofrm` so the child's resulting local transform equals
    // the override, expressed relative to that ancestor. No-op if `objId` is invalid, `rels` is
    // null, or no ancestor with a valid `objId` exists.
    void SetRelations(animINST *pInst, instCONSTRUCTOR_NODE_RELATIONS *rels);

    // 0x826E11A8 — resolve (and cache into `objId`) the objOBJ index matching `objName` the first
    // time this node is shown, then, if `show` is true and the id is in range, mark this node's
    // object index as selected in `selObjs` (via the ObjCB state-condition callback) and flip its
    // `stateProc` bit 8 on for it and its whole sibling/child ring (objOBJ::SetStateProc).
    void Show(animINST *pInst, bool show, dsVECTOR<unsigned char, 8> &selObjs);

    // 0x826E2DA8 — recursively build/attach the object hierarchy for this preset subtree into
    // `pInst`, applying `rels` and marking touched object indices in the caller's "touched"
    // byte-vector (`touched[i]!=0` means keep object i in Construct's prune pass). Picks either a
    // single weighted-random child (exclusive) or rolls each child independently (non-exclusive),
    // using a deterministic per-name/per-instance seed vector fed to m3dVRandIntRange.
    void ConstructInst(animINST *pInst, instCONSTRUCTOR_NODE_RELATIONS *rels,
                        dsVECTOR<unsigned char, 8> &selObjs);
} instCONSTRUCTOR_NODE;
