// 0x827495D8  ?ApplyLipSync@mdlLIP_SYNCER@@UAAXXZ  (vtbl slot 1).
// Apply the current phoneme blend to every driven face object. For each object: seed an identity
// TRS (from the closed idle pose when useClosedAsIdle, else from the object's current model
// matrix), then incrementally blend in each active phoneme sequence weighted by its normalized
// coefficient and the global ease factor, and write the result back into the object's model matrix.
//
// Deviation: the DB decompile lost register allocation ("local variable allocation has failed"),
// which garbled the animExtractObjAnim out-pointer args into (animTRS*)LODWORD(m3dVZero.x/.y).
// The real targets are the two stack TRS locals (trs / trsPhon); reconstructed here. The debug
// STRONG_ASSERT bounds-crash calls (IGNORE_STRONG_ASSERT-gated) are omitted as debug-only.
#include "../../headers/ws/mdl/mdlLIP_SYNCER.h"
#include "../../headers/ws/mdl/mdl_lip_syncer_boundary.h"
#include "../../headers/ws/m3d/m3d_boundary.h"  // m3dClamp

void mdlLIP_SYNCER::ApplyLipSync()
{
    if (!status)  // status == OFF
        return;

    float ease = m3dClamp(1.0f, 0.0f, lipsync::__maxLipSyncBlendTime, 0.0f, timeEaseCur);

    float coefSum = 0.0f;
    for (int i = 0; i < 5; ++i)
        coefSum += seqList[i].coef;

    float invSum = 1.0f;
    if (coefSum <= 0.000001f)
        ease = 0.0f;
    else
        invSum = 1.0f / coefSum;

    float weights[5];
    for (int i = 0; i < 5; ++i)
        weights[i] = seqList[i].coef * invSum * ease;

    for (int objIdx = 0; objIdx < objList.nElem; ++objIdx) {
        objOBJ *obj = objList[objIdx];

        animTRS trs;
        trs.visibility = 1.0f;
        trs.rot.s = 1.0f;
        trs.type.state = 0;
        trs.trans = m3dVZero;
        trs.scale = m3dVZero;
        trs.rot.v = m3dVZero;

        if (useClosedAsIdle) {
            animExtractObjAnim(obj, seqList.list[4].pSeq, seqList.list[4].pSeq->startFrame, &trs);
        } else {
            trs.SetMatr(&obj->matrModel, 0xEu);
        }

        float blendAccum = 0.0f;
        for (int phon = 0; phon < 5; ++phon) {
            float w = weights[phon];
            if (lipsync::__dbgPhonemeId == phon)
                w = 1.0f;
            else if (lipsync::__dbgPhonemeId != -1 || w < 0.000001f)
                continue;

            animSEQ *pSeq = seqList.list[phon].pSeq;
            if (!pSeq || (pSeq->stateSeq & 0x100) == 0)
                continue;

            animTRS trsPhon;
            trsPhon.visibility = 1.0f;
            trsPhon.rot.s = 1.0f;
            trsPhon.type.state = 0;
            trsPhon.trans = m3dVZero;
            trsPhon.scale = m3dVZero;
            trsPhon.rot.v = m3dVZero;

            animExtractObjAnim(obj, seqList.list[phon].pSeq, seqList.list[phon].pSeq->startFrame, &trsPhon);
            blendAccum += w;
            trs.Blend(&trsPhon, w / blendAccum);
        }

        trs.GetMatr(&obj->matrModel);
    }
}
