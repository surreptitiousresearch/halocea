/* hcex_create_light @0x823E58B0 — spawn a ws-engine light entity for a Blam flashlight light and record
 * it in the sorted hcexLights vector keyed by the Blam light id. Builds the light tag entity name
 * ("lgt_<leaf>" run through hcex_conv_name), creates the entity via entCreate (suspending farm adds
 * around the call), takes a weak reference to it, and inserts {lightId, {ent, id, entLight}} into
 * hcexLights. The ent* / anim* / ds::WEAK_PTR/dsVECTOR template machinery is ws-engine C++ (opaque
 * boundary); refcount inc/dec + operator delete mirror the weak-handle lifetime. */

#include <stdio.h>
#include <string.h>

#include "../headers/hcex/hcex_light.h"
#include "../headers/entCREATE_DATA.h"
#include "../headers/farm_system.h"

typedef struct ent_scene ent_scene;

#define INS_DUP_IGNORE 0

extern ent_scene  *gsScenePtr;
extern farm_system *farmSys;
extern void       *hcexLights;   /* dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8> */
extern "C" int         effectId_0;

extern "C" char      *hcex_conv_name(char *name, int size);
extern "C" int        sprintf_0(char *string, const char *format, ...);
extern void       entCREATE_DATA_ctor(entCREATE_DATA *data);
extern void       animCREATE_DATA_dtor(animCREATE_DATA *data); /* ??1animCREATE_DATA@@UAA@XZ @0x8252CEA0; derived entCREATE_DATA upcasts implicitly */
extern entENTITY *entCreate(ent_scene *scene, const char *tag_name, void *tpl_name, entCREATE_DATA *data, void *msg);
extern void       ds_WEAK_PTR_SetPtr(ds_WEAK_PTR *weak, entENTITY *entity);
extern void       dsVECTOR_hcexLights_InsertSorted(void *vector, const hcex_light_pair *value,
                                                   void *position, int dup_policy);
extern long       osLockedIncrement(volatile int *value);
extern long       osLockedDecrement(volatile int *value);
extern void       operator_delete(void *block);

extern "C" void hcex_create_light(int lightId, int obj_follow, const char *lightName)
{
    ds_WEAK_PTR_HANDLE *handle = 0;
    ds_WEAK_PTR weak;
    weak.pHandle = 0;

    char tag_name[64];
    const char *leaf = strrchr(lightName, '\\');
    snprintf(tag_name, sizeof(tag_name), "lgt_%s", leaf + 1);
    hcex_conv_name(tag_name, 64);

    entCREATE_DATA create_data;
    entCREATE_DATA_ctor(&create_data);
    int instance_id = effectId_0++;
    sprintf_0(create_data.name, "light%d", instance_id); /* DEVIATION: callee is sprintf_0 per bl @0x823E5920 */

    char saved_suspend = farmSys->isSuspendAdd;
    farmSys->isSuspendAdd = 1;
    entENTITY *entity = entCreate(gsScenePtr, tag_name, 0, &create_data, 0);
    farmSys->isSuspendAdd = saved_suspend;

    if ( entity )
    {
        ds_WEAK_PTR_SetPtr(&weak, entity);
        handle = weak.pHandle;
    }

    hcex_light_pair pair;
    pair.key = lightId;
    pair.val.ent.pHandle = handle;
    pair.val.id = lightId;
    if ( handle )
        osLockedIncrement(&handle->refCount);
    pair.val.entLight = (entSFX_LIGHT *)weak.pHandle;  /* v12[1] — second weak slot reused as entLight */

    char comparator = 0;
    dsVECTOR_hcexLights_InsertSorted(&hcexLights, &pair, &comparator, INS_DUP_IGNORE);

    ds_WEAK_PTR_HANDLE *inserted_handle = pair.val.ent.pHandle;
    if ( inserted_handle )
    {
        osLockedDecrement(&inserted_handle->refCount);
        if ( inserted_handle->refCount <= 0 )
            operator_delete(inserted_handle);
    }

    animCREATE_DATA_dtor(&create_data);

    if ( handle )
    {
        osLockedDecrement(&handle->refCount);
        if ( handle->refCount <= 0 )
            operator_delete(handle);
    }
}
