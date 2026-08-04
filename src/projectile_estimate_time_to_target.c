/* projectile_estimate_time_to_target @0x83759758 — estimate a projectile's flight time to a target at
 * target_distance: distance / muzzle speed, or 0 for a non-moving projectile.
 *
 * DEVIATION: distance and the return are routed through doubles by the soft-float ABI; they are floats. */

#include "headers/projectile_definition.h"

float projectile_estimate_time_to_target(const projectile_definition *projectile_definition, float target_distance)
{
    if ( projectile_definition->projectile.initial_velocity <= 0.0f )
        return 0.0f;
    return target_distance / projectile_definition->projectile.initial_velocity;
}
