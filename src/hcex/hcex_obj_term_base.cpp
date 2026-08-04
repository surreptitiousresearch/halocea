/* hcex_obj_term_base @0x823EE700 — tear down a registered Blam object's ws-engine presence.
 * Source: D:\Projects\code\HCEX\sources\halo_object.cpp:151. Finds the HCEX_OBJ for the id, terminates
 * its cinematic binding, walks its follower weak-pointers terminating each live entity, then disposes
 * the object's own animation instance (terminating its entity, or queueing the instance onto
 * hcexDeadInstances when the entity is already gone), erases the HCEX_OBJ, and clears lipsync.
 *
 * Deviation: the decompiler modeled ten junk integer params (a3..a12) from a misread stack frame; only
 * r3 (id) and r4 (isInstantTerm) are real (the hcex_obj_term thunk at 0x823EE8F8 sets r4=0 then tail-
 * calls here). a12 is reused as the search-key local. The dsVECTOR/ds::WEAK_PTR templated calls and the
 * entENTITY/animINST types are ws-engine C++ (extern boundary). Asserts omitted. */

#include "../headers/hcex/HCEX_OBJ.h"
#include "../headers/animINST.h"
#include "../headers/ds_weak_ptr.h"

extern void *hcexObjects;         /* dsVECTOR<HCEX_OBJ,8> */
extern void *hcexDeadInstances;   /* dsVECTOR<animINST *,8> */

extern int          dsVECTOR_HCEX_OBJ_8__FindSorted(void *vector, const int *key, int *out_position);
extern HCEX_OBJ    *dsVECTOR_HCEX_OBJ_8__operatorSubscript(void *vector, int position);
extern void         dsVECTOR_HCEX_OBJ_8__Erase(void *vector, int position, int count);
extern ds_WEAK_PTR *dsVECTOR_WEAK_PTR_entENTITY_8__operatorSubscript(dsVECTOR_followers *vector, int position);
extern void         dsVECTOR_animINSTptr_8__PushBack(void *vector, animINST **value);
extern void         hcex_cine_obj_term(int id);
extern void         hcex_obj_term_ent(HCEX_OBJ *obj, entENTITY *entity, bool isInstantTerm);
extern void         hcex_lipsync_term(int id);
extern long         osLockedIncrement(volatile int *value);
extern long         osLockedDecrement(volatile int *value);
extern void         operator_delete(void *block);

void hcex_obj_term_base(int id, bool isInstantTerm)
{
    int position = 0;
    int index = dsVECTOR_HCEX_OBJ_8__FindSorted(&hcexObjects, &id, &position);
    HCEX_OBJ *obj = dsVECTOR_HCEX_OBJ_8__operatorSubscript(&hcexObjects, index);

    hcex_cine_obj_term(obj->id);

    for ( int i = 0; i < obj->followers.nElem; ++i )
    {
        ds_WEAK_PTR_HANDLE *handle = dsVECTOR_WEAK_PTR_entENTITY_8__operatorSubscript(&obj->followers, i)->pHandle;
        bool dead;
        if ( handle )
        {
            osLockedIncrement(&handle->refCount);
            dead = handle->pPtr == 0;
        }
        else
        {
            dead = true;
        }

        if ( !dead )
        {
            entENTITY *entity = handle ? handle->pPtr : 0;
            hcex_obj_term_ent(0, entity, isInstantTerm);
        }

        if ( handle )
        {
            osLockedDecrement(&handle->refCount);
            if ( handle->refCount <= 0 )
                operator_delete(handle);
        }
    }

    animINST *inst = obj->pInst;
    if ( inst )
    {
        entENTITY *entity = inst->pEnt;  /* animINST::pEnt @0xC (DB-verified) */
        if ( entity )
        {
            hcex_obj_term_ent(obj, entity, isInstantTerm);
        }
        else
        {
            animINST *dead_inst = obj->pInst;
            dsVECTOR_animINSTptr_8__PushBack(&hcexDeadInstances, &dead_inst);
        }
    }

    dsVECTOR_HCEX_OBJ_8__Erase(&hcexObjects, index, 1);
    hcex_lipsync_term(id);
}
