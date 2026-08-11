// HCEX_ANIM_MNG_FP__ApplyAnim @0x823DF1C8
/* HCEX_ANIM_MNG_FP::ApplyAnim @0x823DF1E4 (overrides animMNG::ApplyAnim) — locate this manager's
 * entry in the global fpModels vector by matching pInst; if it has a resolved HCEX_MODEL, recompute
 * every node whose bridge index is valid: invert the node's parent local transform, compose it with
 * the synced Blam node matrix (hcex_make_matr), strip scale, and store into the ws node's
 * matrModel; then recalc every node's LTM regardless. On the first call ever, latch the static
 * fpOfs offset once (lazy-init guard); if `repos` is set, additionally reposition the whole
 * instance (translate + roll/pitch/yaw) and recalc every LTM a second time.
 *
 * Disassembly-verified argument list (the decompiler shows this call with far more int-typed
 * trailing parameters than the underlying method actually takes — the classic reg-alloc-mess
 * artifact for functions carrying a `dsVECTOR<short,8>::operator[]` deep in a loop): every
 * Translate/RotateX/RotateY/RotateZ call here in fact takes exactly (angle/vector, operOrder),
 * confirmed against the callee's own `animINST::RotateX/Y/Z(float,int)` prototypes and the
 * `li r5,1` / `li r6,0` immediates loaded at each call site.
 *
 * fpOfs/rotX/rotY/rotZ/repos are HCEX first-person reposition tuning globals with no prior
 * reference in the corpus (first encountered here); modeled with the types their disassembly
 * loads imply (m3dV, float, float, float, bool). `_S2_0` is the compiler-generated guard byte for
 * the `static m3dV fpOfs = m3dVZero;` function-local static (lazy, guarded init on first call). */

#include "../../headers/hcex/HCEX_ANIM_MNG_FP.h"
#include "../../headers/hcex/HCEX_MODEL.h"
#include "../../headers/fp_model_vector.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsPAIR.h"
#include "../../headers/ws/obj/objITER_TREE.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/ws/m3d/m3dV.h"

extern "C" void *memcpy(void *dst, const void *src, unsigned int n); // boundary — CRT
extern void hcex_make_matr(const hcex_matr4x3 *in, m3dMATR *out);     // boundary
extern void m3dMultiplyMatr(const m3dMATR *a, const m3dMATR *b, m3dMATR *out); // boundary
extern void objCalcNodeLTM(objOBJ *pObjFrom);                          // boundary

#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h" // canonical — avoids C2011 vs headers pulling it in
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

extern dsVECTOR<FP_MODEL, 8>                              fpModels;   // boundary global — first-person model table
extern dsVECTOR<dsPAIR<unsigned int, HCEX_MODEL>, 8>       hcexModels; // boundary global — resolved Halo model table

extern const m3dV m3dVZero; // boundary — zero vector constant

// First-person reposition tuning globals (no prior corpus reference; first touched here).
extern "C" float rotX, rotY, rotZ; // boundary — fixed yaw/pitch/roll offsets applied when `repos` is set
extern "C" bool  repos;             // boundary — whether to apply the fixed fpOfs/rot* reposition

void HCEX_ANIM_MNG_FP::ApplyAnim(unsigned char typeValidation)
{
    int i = 0;
    if ( fpModels.nElem <= 0 )
        return;

    while ( fpModels[i].pInst != pInst )
    {
        if ( ++i >= fpModels.nElem )
            return;
    }

    int modelIdx = fpModels[i].modelIdx;
    if ( modelIdx < 0 )
        return;

    HCEX_MODEL &model = hcexModels[modelIdx].val;

    objITER_TREE iter;
    iter.isSkipped = 0;
    iter.pCur = pInst->pObj;
    iter.pRoot = iter.pCur;

    for ( objOBJ *cur = iter.pCur; cur; cur = iter.pCur )
    {
        short nodeIdx = model.nodeIndices[cur->id];
        if ( nodeIdx >= 0 )
        {
            m3dMATR parentLT;
            memcpy(&parentLT, &cur->parent->matrLT, sizeof(m3dMATR));
            m3dMATR parentLTInv;
            parentLT.Invert(&parentLTInv);

            m3dMATR nodeMatr;
            hcex_make_matr(&sync_data.node_matrices[nodeIdx], &nodeMatr);
            if ( !IGNORE_STRONG_ASSERT && !nodeMatr.Check() )
            {
                // DEVIATION: was a named STRONG_ASSERT_DUMMY temp, which emits `addi r3, r1, off`;
                static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash( // the binary emits `li r3, 0`
                    "matrLT.Check()", "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 3971, empty_string);
            }

            m3dMultiplyMatr(&nodeMatr, &parentLTInv, &cur->matrModel);
            cur->matrModel.RemoveScale();
        }
        objCalcNodeLTM(cur);
        iter.Next();
    }

    static bool s_fpOfsInitGuard = false; // `_S2_0` bit 0
    static m3dV fpOfs;                    // lazy-initialized to m3dVZero on first call
    if ( !s_fpOfsInitGuard )
    {
        s_fpOfsInitGuard = true;
        fpOfs = m3dVZero;
    }

    if ( repos )
    {
        pInst->Translate(&fpOfs, 1, 0);
        pInst->RotateX(rotX, 1);
        pInst->RotateY(rotY, 1);
        pInst->RotateZ(rotZ, 1);

        objITER_TREE repositionIter;
        repositionIter.isSkipped = 0;
        repositionIter.pCur = pInst->pObj;
        repositionIter.pRoot = repositionIter.pCur;
        for ( objOBJ *cur = repositionIter.pCur; cur; cur = repositionIter.pCur )
        {
            objCalcNodeLTM(cur);
            repositionIter.Next();
        }
    }
}
