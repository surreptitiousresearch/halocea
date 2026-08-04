/* hcex_fp_sync @0x823E85B8 — per-frame synchronisation of the HCEX first-person bridge models with the
 * ws-engine scene. It (1) marks every fp model hidden in both split-screen cameras, (2) re-evaluates the two
 * cyborg-fp model pairs (view 0 and view 1) via hcex_update_fp_model, then (3) for each model whose per-camera
 * visibility changed since last frame, either drives the backing entity's show/hide virtual methods or, when
 * the model has no entity, pokes the animation instance's shown-to-view state bits. Each model's instance is
 * additionally flagged 0x800000 (force re-evaluate) every frame.
 *
 * fpModels / animINST are modeled at their database offsets; the entity show/hide calls are virtual dispatch
 * (extern boundary). hcex_update_fp_model is a mangled hcex sibling (?hcex_update_fp_model@@YAXHH@Z). */

#include "../headers/animINST.h"
#include "../headers/fp_model_vector.h"

extern fp_model_vector fpModels;
extern int  hcex_fp_model1[2];
extern int  hcex_fp_model2[2];
extern void hcex_update_fp_model(int model_handle, int cam_index);

extern "C" void hcex_fp_sync(void)
{
    int i;

    /* Default every model to hidden in both cameras before re-evaluating. */
    for ( i = 0; i < fpModels.nElem; ++i )
    {
        fpModels.pData[i].isHiddenCam1 = 1;
        fpModels.pData[i].isHiddenCam2 = 1;
    }

    hcex_update_fp_model(hcex_fp_model1[0], 0);
    hcex_update_fp_model(hcex_fp_model2[0], 0);
    hcex_update_fp_model(hcex_fp_model1[1], 1);
    hcex_update_fp_model(hcex_fp_model2[1], 1);

    for ( i = 0; i < fpModels.nElem; ++i )
    {
        FP_MODEL *model = &fpModels.pData[i];

        /* Camera-1 visibility edge. */
        if ( model->isHiddenCam1Prev != model->isHiddenCam1 )
        {
            animINST *inst = model->pInst;
            if ( inst->pEnt )
            {
                void (**entity_vtable)(entENTITY *) = *(void (***)(entENTITY *))inst->pEnt;
                if ( model->isHiddenCam1 )
                    entity_vtable[98](inst->pEnt);   /* +0x188 — show */
                else
                    entity_vtable[99](inst->pEnt);   /* +0x18C — hide */
            }
            else
            {
                /* shown-to-view-1 bit 0x80 (state2 bits unnamed in DB) */
                inst->state2 = (model->isHiddenCam1 ? 0x80 : 0) | (inst->state2 & ~0x80u);
            }
        }

        /* Camera-2 visibility edge (shown-to-view-2 bit 0x100). */
        if ( model->isHiddenCam2Prev != model->isHiddenCam2 )
        {
            animINST *inst = model->pInst;
            if ( inst->pEnt )
            {
                void (**entity_vtable)(entENTITY *) = *(void (***)(entENTITY *))inst->pEnt;
                if ( model->isHiddenCam2 )
                    entity_vtable[98](inst->pEnt);
                else
                    entity_vtable[99](inst->pEnt);
            }
            else
            {
                inst->state2 = (model->isHiddenCam2 ? 0x100 : 0) | (inst->state2 & ~0x100u);
            }
        }

        model->isHiddenCam2Prev = model->isHiddenCam2;
        model->isHiddenCam1Prev = model->isHiddenCam1;
        model->pInst->state2 |= 0x800000u;
    }
}
