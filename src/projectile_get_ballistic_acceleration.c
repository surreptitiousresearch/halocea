/* projectile_get_ballistic_acceleration @0x83759290 — return the downward acceleration a projectile of this
 * type experiences in air: -(air_gravity_scale * global_gravity).
 *
 * DEVIATION: the decompiler routed the float return through a double (soft-float ABI artifact). */

#include "headers/projectile_definition.h"
#include "headers/blam_data_globals.h"


float projectile_get_ballistic_acceleration(const projectile_definition *projectile_definition)
{
    return -(projectile_definition->projectile.air_gravity_scale * global_gravity);
}
