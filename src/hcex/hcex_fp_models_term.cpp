/* hcex_fp_models_term @0x823DDC50 — terminate all first-person bridge models: for each entry with a live
 * instance, post a destroy message to its entity if it has one, otherwise terminate the instance directly;
 * then empty the vector and clear the cached fp-model slot pointers.
 *
 * hcex-bridge code; ws-engine container/message primitives and the hcex_obj_term_inst helper are externs. */

#include "../headers/animINST.h"
#include "../headers/fp_model_vector.h"

extern fp_model_vector fpModels;
extern struct msgSYSTEM msgSystem;
extern int hcex_fp_model1[2];
extern int hcex_fp_model2[2];

extern void msgSYSTEM_SendMsg(struct msgSYSTEM *system, struct entENTITY *entity, int message, void *a, void *b);
extern void hcex_obj_term_inst(animINST *instance);

void hcex_fp_models_term(void)
{
    for ( int i = 0; i < fpModels.nElem; ++i )
    {
        animINST *instance = fpModels.pData[i].pInst;
        if ( instance )
        {
            if ( instance->pEnt )
                msgSYSTEM_SendMsg(&msgSystem, instance->pEnt, 2, 0, 0);
            else
                hcex_obj_term_inst(instance);
        }
    }
    fpModels.nElem = 0;
    hcex_fp_model1[0] = 0;
    hcex_fp_model2[0] = 0;
    hcex_fp_model1[1] = 0;
    hcex_fp_model2[1] = 0;
}
