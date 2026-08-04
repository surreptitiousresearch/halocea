/* particle_system_update_default @0x8373B468 — default per-tick physics update for a particle system
 * that has no host object (`object_index == -1`): runs generic point-physics against the system's own
 * point_physics tag (referenced from the particle system's definition tag, offset +0x44), if any.
 *
 * DEVIATION: Hex-Rays surfaces the `collision_material_type` argument as an uninitialized local (`v3`)
 * with no visible assignment — disasm shows it's not garbage but a deliberate `NULL`: the compiler
 * reused the just-zeroed `r3` (from `li r3, 0` for the `flags` argument) as a cheap source for
 * `stw r3, var_1C(r1)`, the stack slot backing this 9th call argument, rather than emitting a second
 * `li rX, 0`. `radius`/`dt` are the two float arguments and are passed directly in f1/f2 (no stack
 * backing needed since point_physics_update's prototype is fully typed, not vararg) — `radius` is the
 * literal constant 1.0f (`__real_3f800000`), and `dt` is simply this function's own incoming `dtime`
 * moved from f1 to f2 in the prologue. */

#include <stdint.h>
#include "headers/particle_system_datum.h"
#include "headers/particle_system_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/point_physics_definition.h"
#include "headers/real_vector3d.h"

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern unsigned int point_physics_update(unsigned int flags, const point_physics_definition *definition, location *location, int16_t force_weather_palette_index, real_point3d *position, real_vector3d *translational_velocity, const real_vector3d *translational_force, real_vector3d *collision_normal, int16_t *collision_material_type, float radius, float dt);

void particle_system_update_default(particle_system_datum *system, float dtime)
{
    if ( system->object_index == -1 )
    {
        particle_system_definition *definition = TAG_GET(particle_system_definition, system->definition_index);
        int point_physics_index = definition->system_update_point_physics.index; /* +0x44 */
        if ( point_physics_index != -1 )
        {
            const point_physics_definition *physics_definition = TAG_GET(const point_physics_definition, point_physics_index);
            point_physics_update(0, physics_definition, &system->location, -1, &system->position,
                    &system->velocity, 0, 0, 0, 1.0f, dtime);
        }
    }
}
