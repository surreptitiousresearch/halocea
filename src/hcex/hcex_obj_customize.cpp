/* hcex_obj_customize @0x823DCBE8 — HCEX bridge: switch a tracked object's skin preset to the one whose name
 * matches a tag. Given a Blam object id and a tag index, it resolves the object's ws-engine entity, finds the
 * governing propINST_CONSTRUCTOR (directly, or by scanning a property container's children), and — if the
 * short tag name does not already prefix the current preset — selects the first preset whose name it prefixes
 * and applies it via ChangeSkin. No-op for sentinel (-1) ids.
 *
 * The subobject at entity+0x60 is the typed iaIACTOR::propContainer (DB types_members:
 * propCONTAINER = propBASE base@0, properties@0x28, components@0x3C). Its runtime typeId decides the view:
 * as a propINST_CONSTRUCTOR_DESC the container is reinterpreted as a propINST_CONSTRUCTOR, exactly as the
 * binary does. All prop* / dsTYPE_ID / dsVECTOR types are ws-engine boundaries. */

#include "../headers/hcex/hcex_obj_customize_boundary.h"
#include "../headers/ws/ent/entENTITY.h"      /* typed entity: iaIACTOR::propContainer @0x60 */
#include "../headers/ws/prop/propCONTAINER.h" /* properties @0x28 / components @0x3C */

extern "C" void hcex_obj_customize(int id, int tag_index)
{
    if ( id == -1 || tag_index == -1 )
        return;

    HCEX_OBJ *obj = hcex_obj_get(id);
    if ( !obj )
        return;

    animINST *inst = obj->pInst;
    if ( !inst )
        return;

    entENTITY *entity = inst->pEnt;
    if ( !entity )
        return;

    /* typed (was raw entity+0x60 / +0x28 / +0x3C): the embedded iaIACTOR::propContainer and its
       two child-property vectors, viewed through this file's flat shims. */
    propCONTAINER *container = &entity->propContainer;
    propINST_CONSTRUCTOR *desc = (propINST_CONSTRUCTOR *)container;
    dsVECTOR_propBASE *children1 = (dsVECTOR_propBASE *)&container->properties;
    dsVECTOR_propBASE *children2 = (dsVECTOR_propBASE *)&container->components;

    /* fast out: no child properties carried at all. */
    if ( children1->nElem == 0 && children2->nElem == 0 )
        return;

    propINST_CONSTRUCTOR *ctor;
    if ( dsTYPE_ID_IsDerivedFrom(&desc->typeId, &propINST_CONSTRUCTOR_DESC_TYPE_ID) )
    {
        ctor = desc;
    }
    else
    {
        if ( !dsTYPE_ID_IsDerivedFrom(&desc->typeId, &propCONTAINER_DESC_TYPE_ID) )
            return;

        ctor = 0;
        int i;
        for ( i = 0; i < children1->nElem; ++i )
        {
            ctor = propBASE_GetProperty_INST_CONSTRUCTOR(*dsVECTOR_propBASE_index(children1, i));
            if ( ctor )
                break;
        }
        if ( !ctor )
        {
            for ( i = 0; i < children2->nElem; ++i )
            {
                ctor = propBASE_GetProperty_INST_CONSTRUCTOR(*dsVECTOR_propBASE_index(children2, i));
                if ( ctor )
                    break;
            }
            if ( !ctor )
                return;
        }
    }

    if ( !ctor || ctor->curPresetName.pBuffer->strLen == 0 )
        return;

    const char *tag_name = hcex_tag_get_name(tag_index);
    char *leaf = strrchr(tag_name, '\\');

    char short_name[112];
    strncpy(short_name, leaf + 1, 0x40u);
    short_name[63] = 0;
    hcex_conv_name(short_name, 64);

    dsTSTRING_flat candidate;
    candidate.pBuffer = 0;
    dsTSTRING_UnsafeInit(&candidate, short_name, -1, 0);

    if ( !dsTSTRING_IsStartWith(&candidate, ctor->curPresetName.pBuffer->str, 0, 0) )
    {
        int preset_count = propINST_CONSTRUCTOR_GetPresetCount(ctor);
        int i;
        for ( i = 0; i < preset_count; ++i )
        {
            const dsTSTRING_flat *preset = propINST_CONSTRUCTOR_GetPresetName(ctor, i);
            if ( dsTSTRING_IsStartWith(&candidate, preset->pBuffer->str, 0, 0) )
                propINST_CONSTRUCTOR_ChangeSkin(ctor, propINST_CONSTRUCTOR_GetPresetName(ctor, i));
        }
    }

    if ( --candidate.pBuffer->refCount == 0 )
        dlFree(candidate.pBuffer);
}
