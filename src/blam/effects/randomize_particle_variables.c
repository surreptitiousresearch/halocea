/* randomize_particle_variables @0x8373A460 — roll a particle state's randomized appearance/motion
 * variables from the per-state ranges. scale/animation_rate/rotation_rate are each an independent
 * uniform draw; the four color channels share a single 0..1 lerp factor so the color stays coherent.
 *
 * Particle state record (376 bytes, float index view): scale range [18]/[19], animation_rate range
 * [20]/[21], rotation_rate range [22]/[23], color min [24..27], color max [28..31]. */

#include "headers/particle_system_type.h"
#include "headers/particle_system_type_particle_state.h"
#include "headers/particle_system_type_particle_state_interpolated_randomized_variables.h"

#include <stdint.h>
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

void randomize_particle_variables(particle_system_type *type_definition,
        particle_system_type_particle_state_interpolated_randomized_variables *variables, int16_t state_index)
{
    particle_system_type_particle_state *state =
        &((particle_system_type_particle_state *)type_definition->particle_states.address)[state_index];
    float color_fraction = real_seed_random_range(get_global_local_random_seed_address(), 0.0f, 1.0f);

    variables->animation_rate = real_seed_random_range(get_global_local_random_seed_address(),
            state->animation_rate_lower_bound, state->animation_rate_upper_bound);
    variables->rotation_rate = real_seed_random_range(get_global_local_random_seed_address(),
            state->rotation_rate_lower_bound, state->rotation_rate_upper_bound);
    variables->scale = real_seed_random_range(get_global_local_random_seed_address(),
            state->scale_lower_bound, state->scale_upper_bound);
    variables->color.n[0] = real_seed_random_range(get_global_local_random_seed_address(),
            state->color_lower_bound.n[0], state->color_upper_bound.n[0]);
    variables->color.n[1] = ((state->color_upper_bound.n[1] - state->color_lower_bound.n[1]) * color_fraction) + state->color_lower_bound.n[1];
    variables->color.n[2] = ((state->color_upper_bound.n[2] - state->color_lower_bound.n[2]) * color_fraction) + state->color_lower_bound.n[2];
    variables->color.n[3] = ((state->color_upper_bound.n[3] - state->color_lower_bound.n[3]) * color_fraction) + state->color_lower_bound.n[3];
}
