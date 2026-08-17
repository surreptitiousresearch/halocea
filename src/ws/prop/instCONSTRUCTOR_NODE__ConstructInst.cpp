/* ?ConstructInst@instCONSTRUCTOR_NODE@@QAAXPAVanimINST@@PAUinstCONSTRUCTOR_NODE_RELATIONS@@AAV?$dsVECTOR@E$07@@@Z @0x826E2DA8 */
#include "../../headers/ws/prop/instCONSTRUCTOR_NODE.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

extern "C" void dlFree(void *ptr); // boundary — Saber debug allocator free

// Per-instance "random stride" seed (?sRandomStride@@3HA) mixed with objId to decorrelate
// siblings that share the same name/position. boundary global — not reversed in this batch.
extern "C" int sRandomStride;

// instCONSTRUCTOR_NODE::ConstructInst(animINST*, instCONSTRUCTOR_NODE_RELATIONS*,
// dsVECTOR<unsigned char,8>&) — 0x826E2DA8
//
// CAVEAT: the decompile packs the per-character hash accumulation through a chain of aliased
// 64-bit scratch temporaries (v10/v13-v15/v35-v37 sharing register/stack slots across LODWORD/
// HIDWORD/SBYTE3 views of the same value). Disasm confirms every one of those views resolves to
// the SAME single character read (dsTSTRING<char>::operator[] called three times per iteration,
// each a pure read of `name[i]`) — untangled below into one `nameChar` per iteration. The
// dsTSTRING<char> destructor tail (refCount-- ; dlFree when it hits zero) is likewise inlined by
// the compiler rather than calling ~dsTSTRING(); reproduced verbatim here rather than routed
// through the (boundary) destructor, since that is what the disassembly actually executes.
void instCONSTRUCTOR_NODE::ConstructInst(animINST *pInst, instCONSTRUCTOR_NODE_RELATIONS *rels,
                                          dsVECTOR<unsigned char, 8> &selObjs)
{
    m3dV seed;
    pInst->GetPos(&seed);

    const char *instName = pInst->name ? pInst->name : empty_string;

    // Build a private copy of the instance name (so we can index it) and fold every character
    // into the seed vector: a simple per-axis weighted hash of the name string.
    dsTSTRING<char> nameCopy;
    nameCopy.UnsafeInit(instName, -1, 0);
    for (int i = 0; i < nameCopy.pBuffer->strLen; ++i) {
        char nameChar = nameCopy[i];
        seed.x += (float)nameChar * 13.1f;
        seed.y -= (float)nameChar * 53.099998f;
        seed.z += (float)nameChar * 2.0999999f;
    }

    // Mix in a per-node/per-instance stride so siblings with identical names/positions still
    // decorrelate, then fold it into the seed a second time (note: z and x are RE-SCALED by the
    // seed value here, not just offset, unlike the additive per-character pass above).
    int randomSeed = objId + sRandomStride;
    seed.y = (float)randomSeed * 287.54001f + seed.y;
    seed.z = (float)randomSeed * seed.z * -33.75f;
    seed.x = (float)randomSeed * seed.x * 4.3400002f;

    int maxRoll = childProbSum > 0 ? childProbSum - 1 : 0;
    int pickedWeight = m3dVRandIntRange(seed, 0, maxRoll, 1);

    int nChildren = childs.nElem;
    if (exclusive) {
        // Weighted pick: walk children accumulating `prob`, and treat every child whose
        // cumulative range does NOT contain `pickedWeight` as hidden; the one that does is shown,
        // relation-adjusted, and recursively constructed.
        int cumulative = 0;
        for (int i = 0; i < nChildren; ++i) {
            instCONSTRUCTOR_NODE *child = childs[i];
            if (pickedWeight < cumulative || pickedWeight >= child->prob + cumulative) {
                child->Show(pInst, false, selObjs);
            } else {
                child->Show(pInst, true, selObjs);
                child->SetRelations(pInst, rels);
                child->ConstructInst(pInst, rels, selObjs);
            }
            cumulative += child->prob;
        }
    } else {
        // Independent per-child coin flip against each child's own `prob` (0-99 roll).
        for (int i = 0; i < nChildren; ++i) {
            int roll = m3dVRandIntRange(seed, 0, 99, 3);
            instCONSTRUCTOR_NODE *child = childs[i];
            if (roll >= child->prob) {
                child->Show(pInst, false, selObjs);
            } else {
                child->Show(pInst, true, selObjs);
                child->SetRelations(pInst, rels);
                child->ConstructInst(pInst, rels, selObjs);
            }
        }
    }

    // Inlined dsTSTRING<char> destructor tail for `nameCopy`.
    if (--nameCopy.pBuffer->refCount == 0)
        dlFree(nameCopy.pBuffer);
}
