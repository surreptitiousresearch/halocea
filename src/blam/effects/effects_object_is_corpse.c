/* effects_object_is_corpse @0x836E0D78 — true if a unit object is a settled corpse: a unit (type 1)
 * whose flags word (+262) has the "dead" bit 0x4, with a valid death time (+1052) that is more than
 * 30 ticks in the past. Used to gate corpse-specific effects. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int game_time_get(void);

uint8_t effects_object_is_corpse(int object_index)
{
    unit_datum *object_data = object_try_and_get_and_verify_type(object_index, object_mask_biped);
    if ( !object_data || (object_data->object.damage_flags & (1u << _object_dead_bit)) == 0 )
        return 0;

    int death_time = object_data->unit.time_of_death;
    if ( death_time == -1 )
        return 0;
    if ( death_time + 30 >= game_time_get() )
        return 0;
    return 1;
}
