/* actor_combat_enable_special_fire_situation @0x837B7834 — test whether an actor may enter a given
 * special-fire situation (1, 2 or 3), based on its combat state. Returns 1 if permitted, 0 otherwise.
 * Raw actor-datum offsets (stride 1828) reproduced from the decompiler. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_target_type.h"
#include "headers/actor_fire_target_type.h"
#include "headers/blam_data_globals.h"


uint8_t actor_combat_enable_special_fire_situation(uint16_t actor_index, int16_t special_fire_situation)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( special_fire_situation == 1 )
    {
        if ( (uint16_t)actor->control.current_fire_target_type != actor_fire_target_prop )
            return 0;
        return actor->target.target_type < actor_target_clear_line_of_sight_enemy ? 0 : 1;
    }
    if ( special_fire_situation == 2 )
    {
        if ( (uint16_t)actor->control.current_fire_target_type || actor->target.target_type < actor_target_uninspected_orphan || !actor->target.target_really_alive )
            return 0;
        return actor->target.since_any_target_visible_timer < 75 ? 0 : 1;
    }
    if ( special_fire_situation != 3 )
        return 0;
    if ( (uint16_t)actor->control.current_fire_target_type != actor_fire_target_prop || actor->target.target_type < actor_target_clear_line_of_sight_enemy )
        return 0;
    if ( !actor->input.vehicle_gunner )
        return 0;
    return 1;
}
