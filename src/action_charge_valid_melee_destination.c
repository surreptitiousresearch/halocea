/* action_charge_valid_melee_destination @0x837F9178 — is a straight-line melee charge to `goal` viable?
 * Resolves a starting point: if the actor has a valid melee target (byte +1156), uses that target's
 * cached position (floats +288/292/296); else if it has an estimate-eligible unit (byte +1192), estimates
 * that unit's body position (unit_estimate_position, mode 1, body position source at actor +1196); else
 * fails outright. From that start point, tests a straight collision ray toward `goal` (flags 0x33,
 * ignore_object_index -1). The collision test's result doesn't actually change the outcome here — this
 * path always returns 1 once a starting point was resolved, reproduced verbatim.
 *
 * DEVIATION: the DB's inferred prototype types `goal` as a plain __int16; disasm confirms it's read as
 * three consecutive floats (a real_point3d), matching the decompiler's own (correct) local rendering. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/collision_test_flags.h"
#include "headers/blam_data_globals.h"


extern void unit_estimate_position(int unit_index, int16_t estimate_mode, const real_point3d *body_position, real_vector3d *desired_facing, real_vector3d *desired_gun_offset, real_point3d *estimated_position);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);

uint8_t action_charge_valid_melee_destination(int actor_index, real_point3d *goal, real_point3d *melee_target_point)
{
    uint8_t have_start_point = 1;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    real_point3d start_point;

    if ( actor->control.path.at_destination )
    {
        start_point.n[0] = actor->input.position.head_position.n[0];
        start_point.n[1] = actor->input.position.head_position.n[1];
        start_point.n[2] = actor->input.position.head_position.n[2];
    }
    else if ( actor->control.path.path.valid )
    {
        unit_estimate_position(actor->meta.unit_index, 1, &actor->control.path.path.endpoint.point, 0, 0, &start_point);
    }
    else
    {
        have_start_point = 0;
    }

    if ( !have_start_point )
        return have_start_point;

    real_vector3d ray;
    ray.n[0] = goal->n[0] - start_point.n[0];
    ray.n[1] = goal->n[1] - start_point.n[1];
    ray.n[2] = goal->n[2] - start_point.n[2];

    collision_result hit;
    /* 0x33 = front|back facing surfaces, ignore breakable surfaces, and structure */
    collision_test_vector(
        (1u << _collision_test_front_facing_surfaces_bit) | (1u << _collision_test_back_facing_surfaces_bit)
            | (1u << _collision_test_ignore_breakable_surfaces_bit) | (1u << _collision_test_structure_bit),
        &start_point, &ray, -1, &hit);
    return 1;
}
