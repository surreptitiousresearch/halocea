/* particle_system_new_particle_explosion @0x8373B6F8 — initialize a newly spawned particle for an
 * particle system type whose creation physics is _particle_system_type_create_explosion. The type's
 * physics_constants tag_block supplies three reals, indexed by explosion_type_definition_physics_constant:
 * xy_spread, z_spread and intensity. A random unit direction has its horizontal (x/y) and vertical (z)
 * components scaled independently by the two spreads, the vertical component is forced upward if the
 * system is a "grounded" explosion, the particle is positioned at the marker offset by the horizontal
 * spread, its rotation axis is set to that same horizontal spread vector, and its final velocity is the
 * scaled direction times the intensity plus the system's own velocity — then the axis rotation is seeded. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/particle_system_datum.h"
#include "headers/particle_system_definition.h"
#include "headers/particle_system_type.h"
#include "headers/explosion_type_definition_physics_constant.h"
#include "headers/ps_particle_datum.h"
#include "headers/object_marker.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern uint32_t *get_global_local_random_seed_address(void);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern float fabsf(float value);

void particle_system_new_particle_explosion(const particle_system_datum *system, int16_t type_index, ps_particle_datum *particle, object_marker *marker)
{
    const particle_system_definition *definition = TAG_GET(particle_system_definition, system->definition_index);
    const particle_system_type *type = &((const particle_system_type *)definition->types.address)[type_index];
    /* DEVIATION: the reconstruction was missing an indirection — it stopped at &types[type_index] + 96 and
     * read the tag_block's own words as its three reals. The binary loads through them:
     * 8373B740 lwz r11,0x60(r7); 8373B744 add r11,r11,r9; 8373B748 lwz r6,0x60(r11); then lfs 0/4/8(r6).
     * 0x60 is physics_constants.address (the tag_block sits at 0x5C), not a bare float triple. */
    const float *physics_constants = (const float *)type->physics_constants.address;

    float xy_spread = physics_constants[_explosion_type_definition_physics_constant_xy_spread];
    float z_spread = physics_constants[_explosion_type_definition_physics_constant_z_spread];
    float intensity = physics_constants[_explosion_type_definition_physics_constant_intensity];

    seed_random_direction3d(get_global_local_random_seed_address(), &particle->velocity);

    float horizontal_x = particle->velocity.n[0] * xy_spread;
    float horizontal_y = particle->velocity.n[1] * xy_spread;
    float vertical_z = particle->velocity.n[2] * z_spread;
    particle->velocity.n[0] = horizontal_x;
    particle->velocity.n[1] = horizontal_y;
    particle->velocity.n[2] = vertical_z;

    if ( system->physics_states.explosion_state.grounded )
        particle->velocity.n[2] = fabsf(vertical_z);

    particle->position.n[0] = marker->matrix.n[3][0] + horizontal_x;
    particle->position.n[1] = marker->matrix.n[3][1] + horizontal_y;

    particle->axis.n[0] = horizontal_x;
    particle->axis.n[1] = horizontal_y;
    particle->axis.n[2] = 0.0f;

    particle->position.n[2] = marker->matrix.n[3][2] + particle->velocity.n[2];

    particle->velocity.n[0] = horizontal_x * intensity + system->velocity.n[0];
    particle->velocity.n[1] = horizontal_y * intensity + system->velocity.n[1];
    particle->velocity.n[2] = particle->velocity.n[2] * intensity + system->velocity.n[2];

    rotate_vector_about_axis(&particle->axis, global_up3d, 1.0f, 0.0f);
}
