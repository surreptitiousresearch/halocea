/* actor_combat_build_grenade_trajectory @0x837B8170 — solve a throwable-grenade trajectory: looks up the
 * grenade type's projectile definition and calls projectile_aim to get an aim vector/speed toward
 * desired_impact_point, then (on success) converts that into an initial velocity vector (aim_vector *
 * aim_speed) and reports the projectile's ballistic acceleration, when the caller asked for either.
 * Returns whether an aim solution was found.
 *
 * DEVIATION: the decompiler renders this with 36 parameters (a10..a35, a37 unused, mostly phantom register
 * reads); the DB's 11-parameter prototype is ground truth. Register-level disasm confirmed which DB
 * params map to which of projectile_aim's args (grenade_origin -> origin, desired_impact_point ->
 * target_point, optional_ballistic_fraction_min -> target_ballistic_fraction_min passed straight through,
 * aim_vector -> result_aim_vector, aim_speed -> result_velocity — both direct pass-through, reused as
 * projectile_aim's own output slots); `lob` and `arc_time` are confirmed unread in this function body. */

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

uint8_t actor_combat_build_grenade_trajectory(int16_t grenade_type, const real_point3d *grenade_origin, float velocity_max, const real_point3d *desired_impact_point, float *optional_ballistic_fraction_min, uint8_t lob, real_vector3d *aim_vector, float *aim_speed, float *arc_time, real_vector3d *arc_initial_velocity, float *arc_acceleration)
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
                        nullptr, nullptr, &result_linear) )
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
                        *arc_acceleration = projectile_get_ballistic_acceleration(definition);
                }
            }
        }
    }

    return success;
}
