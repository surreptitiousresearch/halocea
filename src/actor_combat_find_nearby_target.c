/* actor_combat_find_nearby_target @0x837B7BD8 — scatter a combat target point by miss_distance in a random
 * horizontal direction, clamped so the scattered point stays in line of sight. Lifts the point 1.5 units up
 * (along global_up3d) to an elevated reference, picks a random yaw and turns it into a horizontal unit vector,
 * and offsets the elevated point by scatter_direction * miss_distance. It first traces straight up from the
 * target: if that is blocked, the elevated reference collapses back to the ground target. It then traces from
 * the (possibly collapsed) reference toward the scattered point; if that trace hits, the scatter distance is
 * pulled in to just short of the hit (t * miss_distance - 0.1, floored at 0). The final scattered point is
 * written back into target_point.
 *
 * DEVIATION: miss_distance is declared double in the DB prototype but is a single-precision float argument
 * (f31, single-precision ops throughout); declared float here. The fsel at 0x837B7D60 is a clamp to >= 0. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);

void actor_combat_find_nearby_target(real_point3d *target_point, float miss_distance)
{
    real_point3d elevated_point;
    elevated_point.n[0] = (global_up3d->n[0] * 1.5f) + target_point->n[0];
    elevated_point.n[1] = (global_up3d->n[1] * 1.5f) + target_point->n[1];
    elevated_point.n[2] = (global_up3d->n[2] * 1.5f) + target_point->n[2];

    float random_angle = real_seed_random_range(get_global_random_seed_address(), -PI, PI);
    real_vector3d scatter_direction;
    vector3d_from_angle(&scatter_direction, random_angle);

    real_vector3d up_offset;
    up_offset.n[0] = elevated_point.n[0] - target_point->n[0];
    up_offset.n[1] = elevated_point.n[1] - target_point->n[1];
    up_offset.n[2] = elevated_point.n[2] - target_point->n[2];

    float result_x = (scatter_direction.n[0] * miss_distance) + elevated_point.n[0];
    float result_y = (scatter_direction.n[1] * miss_distance) + elevated_point.n[1];
    float result_z = (scatter_direction.n[2] * miss_distance) + elevated_point.n[2];

    collision_result collision;
    if ( collision_test_vector(0x23u, target_point, &up_offset, -1, &collision) )
    {
        elevated_point.n[0] = target_point->n[0];
        elevated_point.n[1] = target_point->n[1];
        elevated_point.n[2] = target_point->n[2];
    }

    real_vector3d trace_vector;
    trace_vector.n[0] = result_x - elevated_point.n[0];
    trace_vector.n[1] = result_y - elevated_point.n[1];
    trace_vector.n[2] = result_z - elevated_point.n[2];
    if ( collision_test_vector(0x23u, &elevated_point, &trace_vector, -1, &collision) )
    {
        float distance = (collision.t * miss_distance) - 0.1f;
        if ( distance < 0.0f )
            distance = 0.0f;
        result_x = (scatter_direction.n[0] * distance) + elevated_point.n[0];
        result_y = (scatter_direction.n[1] * distance) + elevated_point.n[1];
        result_z = (scatter_direction.n[2] * distance) + elevated_point.n[2];
    }

    target_point->n[0] = result_x;
    target_point->n[1] = result_y;
    target_point->n[2] = result_z;
}
