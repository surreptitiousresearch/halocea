/* particle_system_new_particle_jet @0x8373B830 — initialize one particle's velocity/position/axis from a
 * "jet" shape definition (a deeply-nested, still-unresolved tag sub-block reached via
 * TAG_INSTANCE(system->definition_index) -> +96 tag_block -> jet record (128 bytes, indexed by
 * type_index) -> +96 -> the jet shape's 3-float record: [0]=speed, [1]=spread fraction 0..1,
 * [2]=axis-mode selector). Splits speed into a "directed" component (along the marker's forward axis,
 * scaled by 1-spread) and a "random" component (along a fresh random direction, scaled by spread),
 * ticked by 1/30s; both are added to the system's own velocity and the marker's world position is copied
 * straight into the particle. The particle's rotation axis is the cross product of its new velocity with
 * either the marker's forward axis (mode 0) or the global up vector (any other mode). */

#include <stdint.h>
#include "headers/particle_system_datum.h"
#include "headers/ps_particle_datum.h"
#include "headers/object_marker.h"
#include "headers/particle_system_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern uint32_t *get_global_local_random_seed_address(void);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);

void particle_system_new_particle_jet(const particle_system_datum *system, int16_t type_index, ps_particle_datum *particle, object_marker *marker)
{
    /* particle_system_definition.types block (+96); each type record is 128 bytes, +96 = jet shape
     * {speed, spread, axis_mode}. The nested jet record itself has no DB struct, so it is read positionally. */
    float *jet_shape = *(float **)((char *)(TAG_GET(particle_system_definition, system->definition_index))->types.address
            + (type_index << 7) + 96);

    float speed = jet_shape[0];
    float spread = jet_shape[1];
    float axis_mode = jet_shape[2];

    float spread_scale = spread * (speed * SECONDS_PER_TICK);
    float directed_scale = (1.0f - spread) * (speed * SECONDS_PER_TICK);

    real_vector3d random_direction;
    seed_random_direction3d(get_global_local_random_seed_address(), &random_direction);

    particle->velocity.n[0] = random_direction.n[0] * spread_scale
            + directed_scale * marker->matrix.forward.n[0] + system->velocity.n[0];
    particle->velocity.n[1] = marker->matrix.forward.n[1] * directed_scale
            + random_direction.n[1] * spread_scale + system->velocity.n[1];
    particle->velocity.n[2] = marker->matrix.forward.n[2] * directed_scale
            + random_direction.n[2] * spread_scale + system->velocity.n[2];

    particle->position = marker->matrix.position;

    const real_vector3d *axis_reference = (axis_mode == 0.0f) ? &marker->matrix.forward : global_up3d;

    /* axis = axis_reference x particle->velocity (standard cross product) */
    particle->axis.n[0] = axis_reference->n[1] * particle->velocity.n[2] - axis_reference->n[2] * particle->velocity.n[1];
    particle->axis.n[1] = axis_reference->n[2] * particle->velocity.n[0] - axis_reference->n[0] * particle->velocity.n[2];
    particle->axis.n[2] = axis_reference->n[0] * particle->velocity.n[1] - axis_reference->n[1] * particle->velocity.n[0];
}
