/* hcex_obj_term_inst @0x823EE6D0 — queue a scene instance for deferred destruction by appending it to the
 * dead-instance vector; the actual scnSCENE::DestroyInst happens later in hcex_destroy_dead_intances.
 *
 * hcex-bridge code; the ws-engine container push is declared as a boundary extern. */

#include "../headers/animINST.h"
#include "../headers/hcex/hcex_dead_inst_vector.h"

extern hcex_dead_inst_vector hcexDeadInstances;
extern void hcexDeadInstances_PushBack(hcex_dead_inst_vector *vector, animINST **instance);

void hcex_obj_term_inst(animINST *pInst)
{
    animINST *instance = pInst;
    hcexDeadInstances_PushBack(&hcexDeadInstances, &instance);
}
