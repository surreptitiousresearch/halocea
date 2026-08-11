/* hcex_obj_definition_name @0x83682288 — return the tag path string of an object's definition tag. Resolves
 * the object handle to its object header, reads the definition tag index at offset 0 (((int *)object)[0]), and returns
 * its tag path. Returns null when the handle is invalid or the definition index is the null datum (-1).
 *
 * object_try_and_get_and_verify_type and tag_get_name are Blam boundaries (valid_type_flags 0xFFFFFFFF = any
 * object type). */

#include <stdint.h>
#include "../headers/object_datum.h"
#include "../headers/object_type.h"

extern "C" void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern "C" char *tag_get_name(int tag_index);

extern "C" char *hcex_obj_definition_name(int object_index)
{
    object_datum *object = (object_datum *)object_try_and_get_and_verify_type(object_index, object_mask_all);
    if ( !object )
        return 0;

    int definition_index = object->definition_index;    /* object+0 */
    if ( definition_index == -1 )
        return 0;

    return tag_get_name(definition_index);
}
