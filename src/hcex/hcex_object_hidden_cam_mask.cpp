/* hcex_object_hidden_cam_mask @0x83683218 — compute the "hidden from camera" bitmask for an object in HCEX
 * remastered rendering, walking up the object's parent chain. Verifies the object exists (any type); when the
 * object has a real model (its model tag index @ definition[13] != -1, or it has no definition) AND its object
 * flags (dword @ +16) have bit 0 set, the object is fully hidden (mask 3). Otherwise it defers to the Blam
 * object_hidden_cam_mask for this object and, if the object is attached to a parent (index @ +284), ORs in the
 * recursively-computed mask of the parent. Returns 0 when the object is invalid. Raw offsets retained — no DB
 * struct covers the runtime object datum. */

#include "../headers/global_tag_instances.h"
#include "../headers/object_flags.h"
#include "../headers/object_datum.h"
#include "../headers/_object_definition.h"
#include "../headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int object_hidden_cam_mask(int object_index);   /* Blam @0x83683050 */

extern "C" int hcex_object_hidden_cam_mask(int id)
{
    object_datum *object = (object_datum *)object_try_and_get_and_verify_type(id, object_mask_all);
    if ( !object )
        return 0;

    int definition_index = object->definition_index;
    int model_index = (definition_index == -1)
        ? -1
        : (*(_object_definition **)TAG_INSTANCE(definition_index))->model.index;   /* objdef+52 */

    if ( (definition_index == -1 || model_index != -1) && (object->object.flags & (1u << _object_invisible_bit)) != 0 )   /* flags @ +16 bit0 */
        return 3;

    int parent_index = object->object.parent_object_index;   /* +284 */
    if ( parent_index == -1 )
        return object_hidden_cam_mask(id);

    return hcex_object_hidden_cam_mask(parent_index) | object_hidden_cam_mask(id);
}
