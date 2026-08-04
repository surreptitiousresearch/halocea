/* hcex_destroy_light @0x823DE130 — destroy the HCEX bridge light with the given id: locate its sorted entry,
 * resolve the weak entity handle, post a destroy message to the entity if it is still alive, then erase the
 * entry from the light vector.
 *
 * hcex-bridge code; ws-engine container, weak-pointer and message primitives are declared as boundary externs.
 * Deviation: the one-shot debug TESTER guard around the weak-pointer dereference is elided (it only sets a
 * static "already-tested" flag and has no runtime effect in a release build). */

#include "../headers/hcex/hcex_light.h"
/* DEVIATION: the decompiler modeled HCEX_LIGHT as {ent@0}; the real layout (hcex_light.h) is
 * {id@0, ent@4, entLight@8} — disasm reads ent.pHandle at dsPAIR +8 (== HCEX_LIGHT+4). Reusing the
 * canonical header corrects the offset of the ent.pHandle read below. */

extern void *hcexLights;       /* dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8> */
extern struct msgSYSTEM msgSystem;

extern int  hcexLights_FindSorted(void *vector, int *key, char *cmp);
extern hcex_light_pair *hcexLights_At(void *vector, int index);
extern void hcexLights_Erase(void *vector, int index, int count);
extern void osLockedIncrement(long *counter);
extern void ds_WEAK_PTR_dtor(ds_WEAK_PTR *weak);
extern void msgSYSTEM_PostMsg(struct msgSYSTEM *system, entENTITY *entity, int message, void *arg);

extern "C" void hcex_destroy_light(int lightId)
{
    char cmp = 0;
    int index = hcexLights_FindSorted(&hcexLights, &lightId, &cmp);
    if ( index < 0 )
        return;

    ds_WEAK_PTR weak;
    weak.pHandle = hcexLights_At(&hcexLights, index)->val.ent.pHandle;
    ds_WEAK_PTR_HANDLE *handle = weak.pHandle;

    int dead;
    if ( handle )
    {
        osLockedIncrement((long *)&handle->refCount); // refCount is int; osLockedIncrement takes long* (same width)
        dead = handle->pPtr == 0;
    }
    else
    {
        dead = 1;
    }

    if ( !dead )
    {
        entENTITY *entity = handle ? handle->pPtr : 0;
        msgSYSTEM_PostMsg(&msgSystem, entity, 2, 0);  /* ws-engine entity message id (boundary; no DB enum found) */
    }

    hcexLights_Erase(&hcexLights, index, 1);
    ds_WEAK_PTR_dtor(&weak);
}
