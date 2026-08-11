/* biped_falling_danger @0x837AE160 — once a biped has been airborne long enough, throttled to roughly
 * once every 15 ticks, makes it scream if it is falling toward a drop that would exceed the global
 * falling-damage killing velocity (estimated from its current downward speed plus the gravity it would
 * gain over the distance to the ground below). Bipeds whose tag opts out (_biped_flying_bit /
 * _biped_immune_to_falling_damage_bit), that are already dead/seated/in a no-scream animation state,
 * are skipped.
 *
 * DEVIATION: the biped_find_ground_surface call sets only r5/r6 (direction = global_down3d, output
 * point), leaving the surface-normal arg (r7) unset; reproduced as nullptr (the callee guards it). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/game_globals_definition.h"
#include "headers/game_globals_falling_damage.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/biped_definition_flags.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/unit_scream_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
extern int game_time_get(void);
extern int biped_find_ground_surface(int biped_index, float ground_distance, const real_vector3d *ground_direction, real_point3d *surface_point, real_vector3d *surface_normal);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern int unit_scream(int unit_index, int16_t scream_type);

void biped_falling_danger(int biped_index)
{
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);

    if ((biped->object.damage_flags & (1u << _object_dead_bit)) != 0)
        return;

    unsigned int definition_flags = TAG_GET(biped_definition, biped->definition_index)->biped.flags;
    if ((definition_flags & (1u << _biped_flying_bit)) != 0
        || (biped->unit.flags & (1u << _unit_no_falling_damage_bit)) != 0
        || (definition_flags & (1u << _biped_immune_to_falling_damage_bit)) != 0
        || biped->unit.actor_index == -1
        || (uint8_t)biped->unit.animation.state == _unit_state_ai_impulse)
        return;

    int now = game_time_get();
    if (biped->biped.airborne_ticks <= 30)
        return;

    int last_check = biped->biped.last_falling_communication_time;
    if (last_check != -1 && last_check + 15 >= now)
        return;

    char dangerous = 1;
    const game_globals_falling_damage *falling_damage =
        (const game_globals_falling_damage *)global_game_globals->falling_damage.address;
    biped->biped.last_falling_communication_time = now;

    real_point3d ground_point;
    if (biped_find_ground_surface(biped_index, 6.0f, global_down3d, &ground_point, nullptr) != -1)
    {
        real_point3d origin;
        object_get_origin(biped_index, &origin);
        float vertical_velocity = biped->object.translational_velocity.n[2];
        float impact_speed_sq = (origin.n[2] - ground_point.n[2]) * global_gravity * 2.0f
                              + vertical_velocity * vertical_velocity;
        if (vertical_velocity > 0.0f || impact_speed_sq < falling_damage->runtime_maximum_damage_velocity * falling_damage->runtime_maximum_damage_velocity)
            dangerous = 0;
    }

    if (dangerous)
        unit_scream(biped_index, _unit_scream_falling);
}
