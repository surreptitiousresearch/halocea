/* particle_system_new_particle_jet @0x8373B830 — initialize one particle's velocity/position/axis for a
 * particle system type whose creation physics is _particle_system_type_create_jet. The type's
 * physics_constants tag_block supplies three reals, indexed by jet_type_definition_physics_constant:
 * velocity, spread fraction (0..1) and a "rotates up" flag. Velocity is split into a directed component
 * (along the marker's forward axis, scaled by 1-spread) and a random component (along a fresh random
 * direction, scaled by spread), ticked by 1/30s; both are added to the system's own velocity and the
 * marker's world position is copied straight into the particle. The particle's rotation axis is the cross
 * product of a reference axis with the new velocity — the global up vector when "rotates up" is set,
 * the marker's forward axis otherwise. */

#include <stdint.h>
#include "headers/particle_system_datum.h"
#include "headers/ps_particle_datum.h"
#include "headers/object_marker.h"
#include "headers/particle_system_definition.h"
#include "headers/particle_system_type.h"
#include "headers/jet_type_definition_physics_constant.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern uint32_t *get_global_local_random_seed_address(void);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);

void particle_system_new_particle_jet(const particle_system_datum *system, int16_t type_index, ps_particle_datum *particle, object_marker *marker)
{
    const particle_system_definition *definition = TAG_GET(particle_system_definition, system->definition_index);
    const particle_system_type *type = &((const particle_system_type *)definition->types.address)[type_index];
    /* DEVIATION: the decompiler's second "+96" is types[type_index].physics_constants.address — the
     * tag_block at 0x5C, not a bare pointer field (8373B884 lwz r11,0x60(r5); 8373B88C lwz r4,0x60(r11)). */
    const float *physics_constants = (const float *)type->physics_constants.address;

    float velocity = physics_constants[_jet_type_definition_physics_constant_velocity];
    float spread_fraction = physics_constants[_jet_type_definition_physics_constant_spread_fraction];
    float rotates_up = physics_constants[_jet_type_definition_physics_constant_rotates_up];

    float spread_scale = spread_fraction * (velocity * SECONDS_PER_TICK);
    float directed_scale = (1.0f - spread_fraction) * (velocity * SECONDS_PER_TICK);

    real_vector3d random_direction;
    seed_random_direction3d(get_global_local_random_seed_address(), &random_direction);

    particle->velocity.n[0] = random_direction.n[0] * spread_scale
            + directed_scale * marker->matrix.forward.n[0] + system->velocity.n[0];
    particle->velocity.n[1] = marker->matrix.forward.n[1] * directed_scale
            + random_direction.n[1] * spread_scale + system->velocity.n[1];
    particle->velocity.n[2] = marker->matrix.forward.n[2] * directed_scale
            + random_direction.n[2] * spread_scale + system->velocity.n[2];

    particle->position = marker->matrix.position;

    const real_vector3d *axis_reference = (rotates_up != 0.0f) ? global_up3d : &marker->matrix.forward;

    /* axis = axis_reference x particle->velocity (standard cross product) */
    particle->axis.n[0] = axis_reference->n[1] * particle->velocity.n[2] - axis_reference->n[2] * particle->velocity.n[1];
    particle->axis.n[1] = axis_reference->n[2] * particle->velocity.n[0] - axis_reference->n[0] * particle->velocity.n[2];
    particle->axis.n[2] = axis_reference->n[0] * particle->velocity.n[1] - axis_reference->n[1] * particle->velocity.n[0];
}
