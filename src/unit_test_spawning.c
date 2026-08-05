/* unit_test_spawning @0x836C8FC8 — spawn the "spawned actors" the unit definition releases on death (e.g.
 * the infection forms from a Flood carrier), once per unit (gated by unit.flags bit 0x20000). The
 * count is a random range from unit.spawn_actor_lower/upper_bound and the spawned actors get the unit's
 * velocity scaled by unit.spawn_throw_velocity. Returns the number actually spawned. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"

extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern int actors_spawn_from_unit(int unit_index, int actor_variant_definition_index, int16_t count, float velocity);

int unit_test_spawning(int unit_index)
{
    int result = 0;
    unit_datum *object_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ( (object_data->unit.flags & (1u << _unit_spawned_actors_bit)) == 0 )  /* not yet spawned */
    {
        unit_definition *definition = TAG_GET(unit_definition, object_data->definition_index);
        if ( definition->unit.spawned_actor_variant.index != -1 )
        {
            int16_t low = definition->unit.spawn_actor_lower_bound;
            int16_t high = definition->unit.spawn_actor_upper_bound + 1;
            result = seed_random_range(get_global_random_seed_address(), low, high);
            if ( (int16_t)result > 0 )
                result = actors_spawn_from_unit(unit_index, definition->unit.spawned_actor_variant.index, result,
                             definition->unit.spawn_throw_velocity * SECONDS_PER_TICK);
            object_data->unit.flags |= (1u << _unit_spawned_actors_bit);
        }
    }
    return result;
}
