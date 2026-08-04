/* update_turret_physics @0x8375F5C0 — tail-call forwarder (confirmed via disasm/decompiler, not a bare
 * blr). Forwards the caller's `powered_mass_points` argument into physics_update's `mass_points` slot
 * (register shuffle: `mr r5, r4`); physics_update's own `powered_mass_points`/`magic_force`/`magic_torque`
 * are forced NULL. This function's own `mass_points` parameter is discarded — never read. */

#include "headers/powered_mass_point_datum.h"
#include "headers/mass_point_datum.h"
#include "headers/real_vector3d.h"

#include "headers/real_vector3d.h"
extern void physics_update(int object_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points, const real_vector3d *magic_force, const real_vector3d *magic_torque);

void update_turret_physics(int vehicle_index, powered_mass_point_datum *powered_mass_points,
        mass_point_datum *mass_points)
{
    physics_update(vehicle_index, 0, (mass_point_datum *)powered_mass_points, 0, 0);
}
