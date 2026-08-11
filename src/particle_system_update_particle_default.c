/* particle_system_update_particle_default @0x8373B528 — advance one particle of a particle-system type through
 * its point-physics for this frame. It resolves the type's runtime record and the particle's current animation
 * state record; when the particle has no next state (index -1) it drives physics with the current state's
 * physics definition and radius directly, otherwise it interpolates radius and the physics definition between
 * the current and next state records by the particle's normalized state fraction (age/duration, clamped to
 * [0,1]). point_physics_update integrates position/velocity, and if it reports a collision whose bit the type's
 * flags opt into (dies_in_air 0x20 / dies_in_water 0x10 / dies_on_ground 0x40), the particle's valid flag is cleared.
 *
 * DEVIATION 1: the DB prototype is (system, type_index, float dtime, ps_particle_datum *particle); Hex-Rays
 * split the r6 particle pointer into a phantom 4th param plus an `a5` alias (disasm 0x8373B560 `mr r31,r6`), so
 * every `a5` here is `particle`. DEVIATION 2: the point_physics_update collision_material_type argument is a
 * null pointer (disasm 0x8373B668 stores r29=0 to the outgoing stack slot), not the uninitialized `v17` local
 * Hex-Rays invented. Runtime-record and state-record fields are raw offsets (no sub-field header covers them). */

#include <stdint.h>
#include "headers/particle_system_datum.h"
#include "headers/particle_system_type.h"
#include "headers/particle_system_type_flags.h"
#include "headers/particle_system_type_particle_state.h"
#include "headers/ps_particle_datum.h"
#include "headers/point_physics_definition.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/global_tag_instances.h"
#include "headers/particle_system_definition.h"

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern point_physics_definition * point_physics_definition_interpolate(const point_physics_definition *physics1, const point_physics_definition *physics2, float t, point_physics_definition *out);
extern unsigned int point_physics_update(unsigned int flags, const point_physics_definition *definition, location *location, int16_t force_weather_palette_index, real_point3d *position, real_vector3d *translational_velocity, const real_vector3d *translational_force, real_vector3d *collision_normal, int16_t *collision_material_type, float radius, float dt);

void particle_system_update_particle_default(const particle_system_datum *system, int16_t type_index, float dtime, ps_particle_datum *particle)
{
    int16_t next_state_index = particle->transition_state_index; /* recovered: *(__int16*)((char*)particle+10) -> transition_state_index */

    particle_system_type *type_runtime = (particle_system_type *)(TAG_GET(particle_system_definition, system->definition_index))->types.address + type_index;   /* def+96 */
    particle_system_type_particle_state *state_records = (particle_system_type_particle_state *)type_runtime->particle_states.address;
    particle_system_type_particle_state *current_state = &state_records[particle->state_index]; /* recovered: *(__int16*)((char*)particle+8) -> state_index */
    const particle_type *type = &system->types[type_index]; /* const: points into const system's inline types[] array, read-only here 2026-07-31 (C4090) */

    const point_physics_definition *physics;
    point_physics_definition interpolated_physics;   /* frame-local blend target (r1+0x70), see below */
    float radius;

    if ( next_state_index == -1 )
    {
        physics = TAG_GET(const point_physics_definition, current_state->point_physics.index);
        radius = ((current_state->variables.radius
                    * type->variables.particle_state_multipliers.radius) * type_runtime->variables.radius);
    }
    else
    {
        float state_fraction = particle->time_left_in_state / particle->state_length; /* recovered: (char*)particle+12,+16 -> time_left_in_state/state_length */
        particle_system_type_particle_state *next_state = &state_records[next_state_index];
        if ( state_fraction >= 0.0f )
        {
            if ( state_fraction > 1.0f )
                state_fraction = 1.0f;
        }
        else
        {
            state_fraction = 0.0f;
        }

        radius = ((((current_state->variables.radius * state_fraction)
                        + (next_state->variables.radius * ((float)1.0 - state_fraction)))
                    * system->types[type_index].variables.particle_state_multipliers.radius)
                * type_runtime->variables.radius);

        /* DEVIATION 3: the `out` slot is r6 = addi r6,r1,0xE0+var_70 @0x8373B630 — a 64-byte stack
         * local, exactly sizeof(point_physics_definition). The tag-instance record IDA labels
         * `# result` (add r5,r8,r11 @0x8373B628) is the UN-SHIFTED slot 3, and only supplies
         * physics2 via lwz r4,0x14(r5); writing the blend there would overwrite the loaded tag
         * instance table every frame. */
        physics = point_physics_definition_interpolate(
            TAG_GET(const point_physics_definition, current_state->point_physics.index),
            TAG_GET(const point_physics_definition, next_state->point_physics.index),
            state_fraction,
            &interpolated_physics);
    }

    unsigned int collision = point_physics_update(0, physics, &particle->location, -1,
        &particle->position, &particle->velocity, 0, 0, 0, radius, dtime); /* recovered: (char*)particle+20/+28/+40 -> location/position/velocity */

    if ( (collision & 1) != 0 && (type_runtime->flags & (1u << _particle_system_type_dies_in_air_bit)) != 0
      || (collision & 2) != 0 && (type_runtime->flags & (1u << _particle_system_type_dies_in_water_bit)) != 0
      || (collision & 4) != 0 && (type_runtime->flags & (1u << _particle_system_type_dies_on_ground_bit)) != 0 )
    {
        particle->valid = 0; /* recovered: *((char*)particle+3) -> valid */
    }
}
