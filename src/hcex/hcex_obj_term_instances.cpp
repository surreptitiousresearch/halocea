/* hcex_obj_term_instances @0x823EDF90 — tear down every tracked HCEX_OBJ's animated ws-engine instance:
 * for any entry whose instance has already lost its entity (pEnt == null), destroy the scene instance
 * directly (scnSCENE::DestroyInst); every entry's pInst is cleared afterward regardless. */

#include "../headers/hcex/hcex_obj_term_instances_boundary.h"

void hcex_obj_term_instances(void)
{
    for ( int i = 0; i < hcexObjects.nElem; ++i )
    {
        animINST *inst = hcexObjects.pData[i].pInst;
        if ( inst && !inst->pEnt )
            scnSCENE_DestroyInst(gsScenePtr, inst);
        hcexObjects.pData[i].pInst = 0;
    }
}
