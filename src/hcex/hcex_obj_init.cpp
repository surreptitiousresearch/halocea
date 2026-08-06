/* hcex_obj_init @0x823EF140 — register a Blam object id in the hcexObjects sorted vector and create its
 * ws-engine animation instance. Source: D:\Projects\code\HCEX\sources\halo_object.cpp:130.
 *
 * Deviation: the decompiler modeled twelve junk integer parameters (a2..a12) from a misread stack
 * frame; only r3 (id) is actually passed (verified at call site 0x836F1B3C: mr r3,r29 / bl). Real
 * signature is hcex_obj_init(int id). The followers/followersOfs dsVECTOR members and the templated
 * FindSorted/InsertSorted/operator[]/~dsVECTOR calls are ws-engine C++ class methods (extern). */

#include "../headers/hcex/HCEX_OBJ.h"
#include "../headers/apCL.h"

extern void *hcexObjects;  /* dsVECTOR<HCEX_OBJ,8> */

extern int       dsVECTOR_HCEX_OBJ_8__FindSorted(void *vector, const int *key, int *out_position);
extern int       dsVECTOR_HCEX_OBJ_8__InsertSorted(void *vector, const HCEX_OBJ *value, int *position, int dup_policy);
extern HCEX_OBJ *dsVECTOR_HCEX_OBJ_8__operatorSubscript(void *vector, int position);
extern void      dsVECTOR_WEAK_PTR_entENTITY_8__dtor(dsVECTOR_followers *vector);
extern "C" void dlFree(void *ptr);
extern "C" int       hcex_obj_model(int id);
extern animINST *hcex_inst_create(int model_index, int id, int unused, int *out_model_idx);

#define INS_DUP_IGNORE 0

extern "C" void hcex_obj_init(int id)
{
    int position = 0;
    apCL cl;
    HCEX_OBJ new_obj[1];

    /* assert hcexObjects.FindSorted(id) < 0 — object not already registered (asserts omitted) */
    dsVECTOR_HCEX_OBJ_8__FindSorted(&hcexObjects, &id, &position);

    new_obj[0].id = id;
    new_obj[0].pInst = 0;
    cl.line = 24;
    cl.file = "D:\\Projects\\code\\common\\INCL.SYS\\ds\\ds_vector.h";
    *(apCL *)&new_obj[0].followersOfs.__cl = cl;
    new_obj[0].followersOfs.pData = 0;
    new_obj[0].followersOfs.nElem = 0;
    new_obj[0].followersOfs.allocated = 0;
    new_obj[0].followers.pData = 0;
    new_obj[0].followers.nElem = 0;
    new_obj[0].followers.allocated = 0;
    HCEX_OBJ_FLAGS(&new_obj[0]) = (HCEX_OBJ_FLAGS(&new_obj[0]) & 0x1FFFF) | HCEX_OBJ_FLAG_IS_INITED;
    *(apCL *)&new_obj[0].followers.__cl = cl;

    int inserted = dsVECTOR_HCEX_OBJ_8__InsertSorted(&hcexObjects, new_obj, &position, INS_DUP_IGNORE);
    HCEX_OBJ *obj = dsVECTOR_HCEX_OBJ_8__operatorSubscript(&hcexObjects, inserted);
    obj->pInst = hcex_inst_create(hcex_obj_model(id), id, 0, &obj->modelIdx);

    dlFree(new_obj[0].followersOfs.pData);
    dsVECTOR_WEAK_PTR_entENTITY_8__dtor(&new_obj[0].followers);
}
