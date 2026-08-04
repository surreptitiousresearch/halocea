/* hcex_obj_term_ent @0x823DDD30 — tear down the ws-engine entity backing a Blam object.
 * Source: D:\Projects\code\HCEX\sources\halo_object.cpp. For an instant term, synchronously send the
 * entity a destroy message (msg 2). Otherwise, if the entity's container is (or owns) a propHALO_OBJ,
 * call its OwnerTerm and resync the HCEX_OBJ; if it has no properties/components, or is neither a
 * propHALO_OBJ nor a propCONTAINER carrying one, just post the destroy message asynchronously.
 *
 * The prop* / ent* / msg* / dsTYPE_ID/dsVECTOR types are ws-engine C++ (opaque boundary); container fields
 * are reached by the database offsets in propCONTAINER.h. */

#include "../headers/hcex/HCEX_OBJ.h"
#include "../headers/ws/prop/propCONTAINER.h"   /* real ws C++ propCONTAINER/propBASE/dsTYPE_ID */

typedef struct propHALO_OBJ propHALO_OBJ;       /* ws prop subclass — pointer-only boundary here */

extern char msgSystem;
extern dsTYPE_ID propHALO_OBJ_DESC_TYPE_ID;
extern dsTYPE_ID propCONTAINER_DESC_TYPE_ID;

extern void          msgSYSTEM_SendMsg(char *system, entENTITY *entity, int message, void *msg_data, void *msg_data2);
extern void          msgSYSTEM_PostMsg(char *system, entENTITY *entity, int message, void *msg_data);
extern int           dsTYPE_ID_IsDerivedFrom(dsTYPE_ID *type, dsTYPE_ID *base);
extern propBASE     **dsVECTOR_propBASEptr_index(void *vector, int position);
extern propHALO_OBJ *propBASE_GetProperty_propHALO_OBJ(propBASE *prop);
extern void          propHALO_OBJ_OwnerTerm(propHALO_OBJ *halo_obj);
extern void          hcex_obj_sync(HCEX_OBJ *obj);

void hcex_obj_term_ent(HCEX_OBJ *obj, entENTITY *pEnt, bool isInstantTerm)
{
    if ( isInstantTerm )
    {
        msgSYSTEM_SendMsg(&msgSystem, pEnt, 2, 0, 0);
        return;
    }

    propCONTAINER *container = (propCONTAINER *)pEnt;  /* propContainer embedded at entity offset 0 */
    /* recovered: *(int*)(base+0x28+DSVECTOR_NELEM_OFFSET=0x08) read `allocated`; the binary reads
     * dsVECTOR<propBASE*,8>::nElem @+0x04 (see decompile of 0x823DDD30) -> container->properties.nElem */
    int properties_count = container->properties.nElem;
    int components_count = container->components.nElem;
    dsTYPE_ID *typeId = &container->typeId;

    propHALO_OBJ *halo_obj = 0;
    int empty = (properties_count == 0 && components_count == 0);

    if ( !empty )
    {
        if ( dsTYPE_ID_IsDerivedFrom(typeId, &propHALO_OBJ_DESC_TYPE_ID) )
        {
            halo_obj = (propHALO_OBJ *)container;
        }
        else if ( dsTYPE_ID_IsDerivedFrom(typeId, &propCONTAINER_DESC_TYPE_ID) )
        {
            for ( int i = 0; i < properties_count && !halo_obj; ++i )
                halo_obj = propBASE_GetProperty_propHALO_OBJ(*dsVECTOR_propBASEptr_index(&container->properties, i));
            for ( int i = 0; i < components_count && !halo_obj; ++i )
                halo_obj = propBASE_GetProperty_propHALO_OBJ(*dsVECTOR_propBASEptr_index(&container->components, i));
        }
    }

    if ( halo_obj )
    {
        propHALO_OBJ_OwnerTerm(halo_obj);
        if ( obj )
            hcex_obj_sync(obj);
    }
    else
    {
        msgSYSTEM_PostMsg(&msgSystem, pEnt, 2, 0);
    }
}
