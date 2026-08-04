#include <stdint.h>
#include "headers/object_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

BOOL should_track_object(int object_index, uint8_t multiplayer_game)
{
    object_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_unit);

    if (!object)
        return 0;

    return (object->object.damage_flags & (1u << _object_dead_bit)) == 0;
}
