/* hcex_obj_model @0x836821D8 — return the model tag index referenced by a live Blam object id.
 * Verifies the object exists (any type), reads its object_definition tag, and returns the model
 * tag index (object_definition byte 0x34). 0 if the object is invalid. The decompiler types the
 * loaded value as _DWORD*, but it is the model tag index (int) — DB prototype + the caller
 * (hcex_inst_create's int model_index) + disasm (lwz r3,0x34(r8)) all confirm an int return, and
 * this matches the sibling reconstruction src/hcex_obj_model.c. */

#include "../headers/global_tag_instances.h"
#include "../headers/_object_definition.h"
#include "../headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

extern "C" int hcex_obj_model(int id)
{
    int *object_data = (int *)object_try_and_get_and_verify_type(id, object_mask_all);
    if ( !object_data )
        return 0;

    _object_definition *object_definition = *(_object_definition **)TAG_INSTANCE(*object_data);
    return object_definition->model.index;   /* objdef+52 */
}
