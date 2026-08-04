/* actor_combat_reaim_grenade @0x837B7F70 — recompute and validate the actor's grenade throw solution toward
 * its stored target point (actor+1704). Looks up the grenade projectile definition via the game globals
 * grenade table (indexed by the actor's grenade tag's grenade-type, then that entry's projectile tag), asks
 * projectile_aim for an aim vector / muzzle speed / arc time, and accepts it only if:
 *   - the planar (x,y) component of the aim is non-degenerate,
 *   - the aim points within 30 degrees of the actor's facing (+372/+376), dot >= cos30 (0.8660254),
 *   - and the resulting ballistic arc has clear line of fire (ai_test_ballistic_line_of_fire).
 * On success the aim vector and speed are latched into actor+1724/1728/1732/1736 and 1 is returned.
 *
 * The ai_test_ballistic_line_of_fire call was re-derived from disasm 0x837B80FC-0x837B8124 under the
 * float-GPR-shadow ABI (each float arg skips a GPR slot): r3=actor_index, r4=origin, f1=arc_time (r5 is
 * f1's shadow, not a stale register), r6=&throw_velocity (arc_initial_velocity), f2=arc_acceleration
 * (r7 shadow), r8=actor->control.grenade_current_ignore_object_index (actor+0x6B8 — the real 6th arg,
 * not an extra one), r9=(vehicle_index != -1) via the addic/subfe carry idiom. The prior rendering had
 * followed the decompiler's no-shadow arg map, which mis-passed (int)&throw_velocity as
 * ignore_object_index. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/actor_variant_definition.h"
#include "headers/game_globals.h"
#include "headers/game_globals_grenade.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);
extern double __fabs(double x);

typedef struct projectile_definition projectile_definition;

extern uint8_t projectile_aim(const projectile_definition *projectile_definition, const real_point3d *origin, const real_point3d *target_point, const float *override_velocity_max, float *target_velocity_min, float *target_ballistic_fraction_min, float *forced_velocity, uint8_t lob, real_vector3d *result_aim_vector, float *result_velocity, float *result_ticks, float *result_distance, uint8_t *result_linear);
extern float projectile_get_ballistic_acceleration(const projectile_definition *projectile_definition);
extern uint8_t ai_test_ballistic_line_of_fire(int actor_index, const real_point3d *origin, float arc_time, const real_vector3d *arc_initial_velocity, float arc_acceleration, int ignore_object_index, uint8_t ignore_vehicles);

int actor_combat_reaim_grenade(int actor_index, const real_point3d *grenade_origin)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    const struct projectile_definition *projectile_definition = 0;

    /* The disasm loads the tag index from meta+0x58 = variant_definition_index (not definition_index):
     * the grenade type lives in the actor VARIANT tag's grenade_combat block (abs 0x180 -> [192]),
     * a DB-typed field — not the actor tag's opaque unused_ranged region. */
    actor_variant_definition *variant_definition =
        TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);
    short grenade_type = variant_definition->grenade_combat.grenade_type;
    game_globals_grenade *grenades_address = (game_globals_grenade *)global_game_globals->grenades.address;
    game_globals_grenade *grenade_entry = &grenades_address[grenade_type];
    if ( grenade_entry )
    {
        int projectile_tag_index = grenade_entry->projectile.index;
        if ( projectile_tag_index != -1 )
            projectile_definition = TAG_GET(const struct projectile_definition, projectile_tag_index);
    }

    real_vector3d aim_vector;
    float result_velocity;
    float result_ticks;
    float result_distance;
    unsigned __int8 result_linear;

    if ( !projectile_aim(projectile_definition, grenade_origin, &actor->control.grenade_current_target,
                         0, 0, 0, &actor->control.grenade_current_aim_speed, actor->control.grenade_current_lob,
                         &aim_vector, &result_velocity, &result_ticks, &result_distance, &result_linear) )
        return 0;

    float planar_length = __fsqrts(aim_vector.n[0] * aim_vector.n[0] + aim_vector.n[1] * aim_vector.n[1]);
    if ( __fabs(planar_length) < 0.000099999997f )
        return 0;
    if ( planar_length <= 0.0f )
        return 0;

    float norm_x = aim_vector.n[0] * (1.0f / planar_length);
    float norm_y = aim_vector.n[1] * (1.0f / planar_length);
    if ( (actor->input.facing_vector.n[1] * norm_y) + (actor->input.facing_vector.n[0] * norm_x) <= 0.86602539f )
        return 0;

    real_vector3d throw_velocity;
    throw_velocity.n[0] = aim_vector.n[0] * result_velocity;
    throw_velocity.n[1] = aim_vector.n[1] * result_velocity;
    throw_velocity.n[2] = aim_vector.n[2] * result_velocity;

    float arc_acceleration = 0.0f;
    if ( !result_linear )
        arc_acceleration = projectile_get_ballistic_acceleration(projectile_definition);

    if ( !ai_test_ballistic_line_of_fire(actor_index, grenade_origin, result_ticks, &throw_velocity,
                                         arc_acceleration, actor->control.grenade_current_ignore_object_index,
                                         actor->input.vehicle_index != -1) )
        return 0;

    actor->control.grenade_current_aim_vector.n[0] = aim_vector.n[0];
    actor->control.grenade_current_aim_vector.n[1] = aim_vector.n[1];
    actor->control.grenade_current_aim_vector.n[2] = aim_vector.n[2];
    actor->control.grenade_current_aim_speed = result_velocity;
    return 1;
}
