/* hcex_obj_prefix @0x83682230 — return the HCEX name prefix for an object, used when building the recreated
 * asset/model name. Only projectiles ("pjl" = projectile jelly light) get a prefix: if the object exists and
 * its type word (+180, read as _WORD[90]) is object_type_projectile (5), return "pjl_"; otherwise no prefix
 * (null). object_try_and_get_and_verify_type is a Blam boundary function (valid_type_flags 0xFFFFFFFF = any
 * type; == object_mask_all). */

#include "../headers/object_datum.h"
#include "../headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

extern "C" const char *hcex_obj_prefix(int id)
{
    object_datum *object = (object_datum *)object_try_and_get_and_verify_type(id, object_mask_all);
    if (object && object->object.type == object_type_projectile) /* object+180: type word == 5 */
        return "pjl_";
    return 0;
}
