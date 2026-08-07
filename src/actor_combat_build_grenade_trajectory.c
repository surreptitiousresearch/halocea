/* actor_combat_build_grenade_trajectory @0x837B8170 — solve a throwable-grenade trajectory: looks up the
 * grenade type's projectile definition and calls projectile_aim to get an aim vector/speed toward
 * desired_impact_point, then (on success) converts that into an initial velocity vector (aim_vector *
 * aim_speed) and reports the ballistic acceleration (zero for a linear solution), when the caller asked
 * for either. Returns whether an aim solution was found.
 *
 * DEVIATION: 10 parameters, not the 11 previously read here (nor the decompiler's 38). A `lob` between
 * optional_ballistic_fraction_min and aim_vector was a float-slot-skip phantom: velocity_max consumes r5's
 * slot while living in f1, so desired_impact_point is r6 and optional_ballistic_fraction_min r7 — exactly
 * what this body forwards to projectile_aim's target_point (`mr r5, r6`) and target_ballistic_fraction_min
 * (`mr r8, r7`), whose own `lob` is the literal 0. Both ends agree: only slots 8/9 arrive on the stack
 * (arg_54/arg_5C; `stfs f1, arg_24` homes velocity_max at slot 2), and arc_time is r10 -> result_ticks. */

#include <stdint.h>
#include "headers/game_globals_definition.h"
#include "headers/game_globals_grenade.h"
#include "headers/global_tag_instances.h"
#include "headers/projectile_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern uint8_t projectile_aim(const projectile_definition *projectile_definition, const real_point3d *origin, const real_point3d *target_point, const float *override_velocity_max, float *target_velocity_min, float *target_ballistic_fraction_min, float *forced_velocity, uint8_t lob, real_vector3d *result_aim_vector, float *result_velocity, float *result_ticks, float *result_distance, uint8_t *result_linear);
extern float projectile_get_ballistic_acceleration(const projectile_definition *projectile_definition);

uint8_t actor_combat_build_grenade_trajectory(int16_t grenade_type, const real_point3d *grenade_origin, float velocity_max, const real_point3d *desired_impact_point, float *optional_ballistic_fraction_min, real_vector3d *aim_vector, float *aim_speed, float *arc_time, real_vector3d *arc_initial_velocity, float *arc_acceleration)
{
    float velocity_max_copy = velocity_max;
    uint8_t success = 0;

    /* recovered: (char *)grenades.address + 68 * grenade_type -> typed &grenades[grenade_type] */
    game_globals_grenade *grenade_entry = &((game_globals_grenade *)global_game_globals->grenades.address)[grenade_type];
    if ( grenade_entry )
    {
        int projectile_definition_index = grenade_entry->projectile.index;
        if ( projectile_definition_index != -1 )
        {
            const projectile_definition *definition = TAG_GET(const projectile_definition, projectile_definition_index);
            if ( definition )
            {
                uint8_t result_linear;
                if ( projectile_aim(definition, grenade_origin, desired_impact_point, &velocity_max_copy,
                        nullptr, optional_ballistic_fraction_min, nullptr, 0, aim_vector, aim_speed,
                        arc_time, nullptr, &result_linear) )
                {
                    success = 1;
                    if ( arc_initial_velocity )
                    {
                        float speed = *aim_speed;
                        arc_initial_velocity->n[0] = aim_vector->n[0] * speed;
                        arc_initial_velocity->n[1] = aim_vector->n[1] * speed;
                        arc_initial_velocity->n[2] = aim_vector->n[2] * speed;
                    }
                    if ( arc_acceleration )
                    {
                        /* DEVIATION: a linear solution has no ballistic drop — 0x837B8260-0x837B8288
                         * branches on result_linear and the zero arm was missing from the pseudocode. */
                        if ( result_linear )
                            *arc_acceleration = 0.0f;
                        else
                            *arc_acceleration = projectile_get_ballistic_acceleration(definition);
                    }
                }
            }
        }
    }

    return success;
}
