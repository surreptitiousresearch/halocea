/* hcex_obj_scenery_idx_type @0x83682178 — hcex bridge: resolve a Blam object id to its object type and
 * scenery/definition index. Validates the object via object_try_and_get_and_verify_type (any type accepted,
 * mask 0xFFFFFFFF); on success writes the object's 16-bit type (object header +0xB4) and its definition
 * index to the caller's out-params and returns 1. Returns 0 when the id is not a live object. */

#include "../headers/object_datum.h"
#include "../headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

extern "C" int hcex_obj_scenery_idx_type(int id, int *obj_idx, int *obj_type)
{
    object_datum *object = (object_datum *)object_try_and_get_and_verify_type(id, object_mask_all);
    if ( object )
    {
        *obj_type = object->object.type;          /* object type enum */
        *obj_idx = object->object.scenery_idx;    /* scenery/definition index */
        return 1;
    }
    return 0;
}
