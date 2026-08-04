/* collision_move_pill @0x83774D70 — move a "pill" (capsule) collision volume from old_position along
 * old_velocity, colliding against the world: builds a bounding sphere that encloses the swept pill (center
 * offset half the velocity plus half the height above old_position; radius covers half the speed, half the
 * height, and the pill's width) and gathers nearby collision features in it. If any features were found,
 * delegates the actual sweep/response to collision_move_point using them. If none were found (nothing
 * nearby to collide with), takes the simple path: new_position = old_position + old_velocity unchanged,
 * new_velocity = old_velocity unchanged, and returns 0 (no collision).
 *
 * DEVIATION: the decompiler renders this with 39 parameters (a11..a39 mostly phantom) and a bogus
 * `RtlCheckStack12` return value threaded through the math; the DB's 10-parameter prototype is ground
 * truth, and the actual center/radius formulas were reconstructed from a full disasm trace (RtlCheckStack12
 * is just this function's large-stack-frame probe, not a real value). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_feature_list.h"
#include "headers/collision_plane.h"

extern float sqrtf(float value);
extern uint8_t collision_get_features_in_sphere(unsigned int flags, const real_point3d *center, float radius, float height, float width, int ignore_object_index, collision_feature_list *features);
extern int16_t collision_move_point(const real_point3d *old_position, const real_vector3d *old_velocity, const collision_feature_list *features, real_point3d *new_position, real_vector3d *new_velocity, int16_t maximum_collision_count, collision_plane *collisions);

int16_t collision_move_pill(unsigned int flags, const real_point3d *old_position, const real_vector3d *old_velocity,
    float height, float width, int ignore_object_index, real_point3d *new_position, real_vector3d *new_velocity,
    int16_t maximum_collision_count, collision_plane *collisions)
{
    real_point3d center;
    center.n[0] = old_velocity->n[0] * 0.5f + old_position->n[0];
    center.n[1] = old_velocity->n[1] * 0.5f + old_position->n[1];
    center.n[2] = (old_velocity->n[2] * 0.5f + old_position->n[2]) + height * 0.5f;

    float speed = sqrtf(old_velocity->n[0] * old_velocity->n[0] + old_velocity->n[1] * old_velocity->n[1]
                        + old_velocity->n[2] * old_velocity->n[2]);
    float radius = (speed * 0.5f + height * 0.5f) + width;

    collision_feature_list features;
    if ( collision_get_features_in_sphere(flags, &center, radius, height, width, ignore_object_index, &features) )
    {
        return collision_move_point(old_position, old_velocity, &features, new_position, new_velocity,
            maximum_collision_count, collisions);
    }

    new_position->n[0] = old_position->n[0] + old_velocity->n[0];
    new_position->n[1] = old_position->n[1] + old_velocity->n[1];
    new_position->n[2] = old_position->n[2] + old_velocity->n[2];
    new_velocity->n[0] = old_velocity->n[0];
    new_velocity->n[1] = old_velocity->n[1];
    new_velocity->n[2] = old_velocity->n[2];
    return 0;
}
