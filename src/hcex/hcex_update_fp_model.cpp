/* hcex_update_fp_model @0x823E82B8 — per-frame sync of a first-person view-model instance for one
 * camera slot: find (or create) the FP_MODEL tracking record for (modelId, cam_idx), copy this
 * frame's Blam node matrices into its HCEX_ANIM_MNG_FP sync block, push the object-space transform
 * onto the animINST, refresh the per-camera hidden-in-stereo flags, and (when the instance has an
 * entity) toggle its active-camouflage material if that state changed since last frame.
 *
 * DEVIATION: the mangled name (`?hcex_update_fp_model@@YAXHH@Z`) and the PushBack call site (which
 * only sets up (this, val) before the call) confirm this takes exactly two parameters; the
 * decompiler's extra a3/a4 are mis-attributed PushBack internals — dropped here.
 *
 * The HIDWORD(v4)==modelId sentinel value is the decompiler's rendering of a 64-bit register pair
 * that is actually just `int modelId` zero-extended; reconstructed directly as `modelId` below. */

#include <string.h>
#include "../headers/hcex/HCEX_MODEL.h"
#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/dsPAIR.h"
#include "../headers/ws/anim/animINST.h"       /* full ws::animINST (Transform/Invalidate/pAnimMng/perInstLock) */
#include "../headers/ws/os/osLOCK.h"
#include "../headers/ws/ent/entENTITY.h"       /* full ws::entENTITY (implSetMaterial) */
#include "../headers/hcex/hcex_matr4x3.h"      /* hcex_matr4x3 — now depends only on hcex_float3.h, so it no
                                                 * longer drags in a conflicting plain-C m3dMATR/m3dV/objOBJ
                                                 * alongside the full ws:: versions pulled in above */
#include "../headers/hcex/HCEX_ANIM_MNG_FP.h"
#include "../headers/hcex/hcex_render_present_boundary.h" /* gsRENDER_SYSTEM, gsSysRender */
#include "../headers/real_matrix4x3.h"

/* FP_MODEL (DB-verified, 24 bytes) — the full ws::animINST-typed twin of fp_model_vector.h's plain-C
 * FP_MODEL. Restated here (rather than including fp_model_vector.h) because this function needs the
 * full struct animINST (Transform/Invalidate/perInstLock/pAnimMng/pEnt), which conflicts one-per-TU
 * with fp_model_vector.h's simplified opaque animINST stand-in; this project's boundary headers are
 * consistently restated per-consumer rather than shared where the two modeling styles would collide. */
struct FP_MODEL {
    int           plrIdx;           /* 0x00 */
    int           modelId;          /* 0x04 */
    int           modelIdx;         /* 0x08 */
    animINST     *pInst;            /* 0x0C */
    int           isActCamo;        /* 0x10 */
    unsigned char isHiddenCam1;     /* 0x14 */
    unsigned char isHiddenCam1Prev; /* 0x15 */
    unsigned char isHiddenCam2;     /* 0x16 */
    unsigned char isHiddenCam2Prev; /* 0x17 */
};

extern dsVECTOR<FP_MODEL, 8>                         fpModels;
extern dsVECTOR<dsPAIR<unsigned long, HCEX_MODEL>, 8> hcexModels;

extern animINST      *hcex_inst_create(int modelId, int id, bool isFP, int *pModelIdx);
extern void hcex_reset_weapon_state(void);
extern real_matrix4x3 * hcex_get_fp_matrices(int modelId, unsigned int cam_idx);
extern void            hcex_make_inst_matr(const hcex_matr4x3 *in, m3dMATR *out);
extern int hcex_plr_is_act_camo(int idx);

void hcex_update_fp_model(int modelId, int cam_idx)
{
    if (!modelId)
        return;

    /* NOTE: indexed via pData directly rather than operator[] — this dsVECTOR<FP_MODEL,8>
     * instantiation's operator[] has not been reversed as its own subroutine in this batch. */
    int i = 0;
    FP_MODEL *fp = 0;
    for (i = 0; i < fpModels.nElem; ++i) {
        if (fpModels.pData[i].modelId == modelId && fpModels.pData[i].plrIdx == cam_idx) {
            fp = &fpModels.pData[i];
            break;
        }
    }

    if (!fp) {
        FP_MODEL newFp;
        newFp.plrIdx = cam_idx;
        newFp.modelId = modelId;
        newFp.modelIdx = 0;
        newFp.pInst = 0;
        newFp.isActCamo = 0;
        fp = &fpModels.PushBack(newFp);
        fp->plrIdx = cam_idx;
        fp->modelId = modelId;
        fp->isActCamo = 0;
        fp->pInst = hcex_inst_create(modelId, fpModels.nElem, true, &fp->modelIdx);
        hcex_reset_weapon_state();
        fp->isHiddenCam1 = 0;
        fp->isHiddenCam1Prev = 0;
        fp->isHiddenCam2 = 0;
        fp->isHiddenCam2Prev = 0;
    }

    real_matrix4x3 *fpMatrices = hcex_get_fp_matrices(modelId, cam_idx);
    HCEX_MODEL *model = &hcexModels.pData[fp->modelIdx].val;
    animINST *pInst = fp->pInst;
    HCEX_ANIM_MNG_FP *pAnimMng = (HCEX_ANIM_MNG_FP *)pInst->pAnimMng;

    pInst->perInstLock.Lock(0, 0);
    int nodeCount = model->objNames.nElem;
    pAnimMng->sync_data.node_count = nodeCount;
    memcpy(pAnimMng->sync_data.node_matrices, fpMatrices, sizeof(hcex_matr4x3) * nodeCount);
    fp->pInst->perInstLock.Unlock(0, 0);

    m3dMATR instMatr;
    hcex_make_inst_matr((const hcex_matr4x3 *)fpMatrices, &instMatr);
    fp->pInst->Transform(&instMatr, 0);

    if (gsSysRender->isStereo3D) {
        fp->isHiddenCam1 = 0;
    } else if (!cam_idx) {
        fp->isHiddenCam1 = 0;
    } else {
        fp->isHiddenCam2 = 0;
    }

    fp->pInst->Invalidate(2, 1);

    if (fp->pInst->pEnt) {
        int isActCamo = hcex_plr_is_act_camo(cam_idx);
        if (fp->isActCamo != isActCamo) {
            fp->isActCamo = isActCamo;
            /* Second argument is always the empty string at every call site observed; only the
             * first argument (the material to apply) varies with isActCamo. */
            dsTSTRING<char> matSlot;   /* always empty */
            dsTSTRING<char> matValue; /* "act_camo" when entering camo, empty when leaving it */
            matSlot.UnsafeInit(0, -1, 0);
            if (isActCamo)
                matValue.UnsafeInit("act_camo", -1, 0);
            else
                matValue.UnsafeInit(0, -1, 0);
            fp->pInst->pEnt->implSetMaterial(matValue, matSlot);
        }
    }
}
