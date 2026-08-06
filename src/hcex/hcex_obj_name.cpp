/* hcex_obj_name @0x83682F08 — return the MODEL tag path (object_definition.model.name) for a live
 * Blam object id. Verifies the object exists (any type) and reads the tag path straight out of its
 * object_definition's model tag_reference. NULL if the object is invalid.
 *
 * Sibling to hcex_obj_model (which returns object_definition.model.index instead). Same
 * TAG_INSTANCE(*object_data) idiom as that file. */

#include "../headers/global_tag_instances.h"
#include "../headers/_object_definition.h"
#include "../headers/object_type.h"

extern "C" void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

extern "C" char *hcex_obj_name(int id)
{
    int *object_data = (int *)object_try_and_get_and_verify_type(id, object_mask_all);
    if ( !object_data )
        return (char *)object_data;

    _object_definition *definition = *(_object_definition **)TAG_INSTANCE(*object_data);
    return definition->model.name;
}
