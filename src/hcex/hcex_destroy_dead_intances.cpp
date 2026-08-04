/* hcex_destroy_dead_intances @0x823EDF08 — destroy every scene instance queued for deferred removal, then
 * empty the queue. If there is no active scene, just empty the queue.
 *
 * hcex-bridge code; the ws-engine scene/container primitives are declared as boundary externs. */

#include "../headers/animINST.h"
#include "../headers/hcex/hcex_dead_inst_vector.h"

extern hcex_dead_inst_vector hcexDeadInstances;
extern struct scnSCENE *gsScenePtr;
extern void scnSCENE_DestroyInst(struct scnSCENE *scene, animINST *instance);

void hcex_destroy_dead_intances(void)
{
    if ( gsScenePtr )
    {
        for ( int i = 0; i < hcexDeadInstances.nElem; ++i )
            scnSCENE_DestroyInst(gsScenePtr, hcexDeadInstances.pData[i]);
    }
    hcexDeadInstances.nElem = 0;
}
