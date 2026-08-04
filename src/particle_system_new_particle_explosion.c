/* particle_system_new_particle_explosion @0x8373B6F8 — initialize a newly spawned particle for an
 * "explosion"-style particle type: picks a random unit direction, scales its horizontal (x/y) and vertical
 * (z) components independently by the particle type definition's spread values (tag_block[24], 128-byte
 * records, floats at +96/+100/+104 — no dedicated header for this sub-block yet), forces the vertical
 * component upward if the system is a "grounded" explosion, positions the particle at the marker offset by
 * the horizontal spread, sets its rotation axis to that same horizontal spread vector, and finally derives
 * the particle's actual velocity as the (still separately-scaled) direction times the definition's third
 * spread value plus the system's own velocity — then seeds the axis rotation. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/particle_system_datum.h"
#include "headers/particle_system_definition.h"
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
    /* particle_system_definition.types block (+96); each type record is 128 bytes, +96 = explosion shape
     * {horizontal_spread, vertical_spread, velocity_scale}. The nested record has no DB struct, read positionally. */
    float *spread = (float *)((char *)(TAG_GET(particle_system_definition, system->definition_index))->types.address
            + (type_index << 7) + 96);
    float horizontal_spread = spread[0];
    float vertical_spread = spread[1];
    float velocity_scale = spread[2];

    seed_random_direction3d(get_global_local_random_seed_address(), &particle->velocity);

    float horizontal_x = particle->velocity.n[0] * horizontal_spread;
    float horizontal_y = particle->velocity.n[1] * horizontal_spread;
    float vertical_z = particle->velocity.n[2] * vertical_spread;
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

    particle->velocity.n[0] = horizontal_x * velocity_scale + system->velocity.n[0];
    particle->velocity.n[1] = horizontal_y * velocity_scale + system->velocity.n[1];
    particle->velocity.n[2] = particle->velocity.n[2] * velocity_scale + system->velocity.n[2];

    rotate_vector_about_axis(&particle->axis, global_up3d, 1.0f, 0.0f);
}
