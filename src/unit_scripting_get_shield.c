/* unit_scripting_get_shield @0x838009D0 — return a unit's shield fraction (object data +0xE4), 0 if it is
 * dead/destroyed (object flags +0x106 bit 0x4), or -1 if the object does not exist. */

#include "headers/object_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

float unit_scripting_get_shield(int unit_index)
{
    object_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_all);
    if ( !unit )
        return -1.0f;
    if ( (unit->object.damage_flags & (1u << _object_dead_bit)) != 0 )
        return 0.0f;
    return unit->object.shield_vitality;
}
