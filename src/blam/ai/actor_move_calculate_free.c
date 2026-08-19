/* actor_move_calculate_free @0x837C7BE8 — convert a desired movement direction into normalized throttle
 * axes relative to facing: in 3D flight, projects onto the facing/left/up axis triple (built via
 * biped_build_flying_axes) to get forward/strafe/vertical throttle; on the ground, only forward/strafe
 * (throttle.z left at 0) using the facing vector's own x/y and its perpendicular (-y, x). Both paths
 * normalize the result. */

#include <stdint.h>
#include "headers/real_vector3d.h"

extern void biped_build_flying_axes(const real_vector3d *forward_vector, real_vector3d *left_vector, real_vector3d *up_vector);
extern float normalize3d(real_vector3d *v);

void actor_move_calculate_free(uint8_t move_in_3d, const real_vector3d *movement_direction,
        const real_vector3d *facing_direction, real_vector3d *throttle)
{
    if ( move_in_3d )
    {
        real_vector3d left, up;
        biped_build_flying_axes(facing_direction, &left, &up);

        throttle->n[0] = movement_direction->n[0] * facing_direction->n[0]
                        + facing_direction->n[1] * movement_direction->n[1]
                        + facing_direction->n[2] * movement_direction->n[2];
        throttle->n[1] = movement_direction->n[1] * left.n[1]
                        + movement_direction->n[2] * left.n[2]
                        + movement_direction->n[0] * left.n[0];
        throttle->n[2] = movement_direction->n[1] * up.n[1]
                        + movement_direction->n[2] * up.n[2]
                        + movement_direction->n[0] * up.n[0];
        normalize3d(throttle);
    }
    else
    {
        float perpendicular_x = -facing_direction->n[1];
        float perpendicular_y = facing_direction->n[0];

        throttle->n[0] = movement_direction->n[0] * facing_direction->n[0]
                        + movement_direction->n[1] * facing_direction->n[1];
        throttle->n[1] = movement_direction->n[0] * perpendicular_x + movement_direction->n[1] * perpendicular_y;
        throttle->n[2] = 0.0f;
        normalize3d(throttle);
    }
}
