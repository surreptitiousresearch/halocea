/* particle_system_next_particle_state_index @0x8373A658 — pick the next state for a system particle.
 * Steps the state index forward or backward (per states_moving_forward), and if it runs off the end of
 * the particle_states block, either bounces (particle_states_loop_forward_backward: clamp and reverse
 * direction), wraps to 0 (particle_states_loop only), or kills the particle (no looping flag). */

#include <stdint.h>
#include "headers/ps_particle_datum.h"
#include "headers/particle_system_type.h"
#include "headers/particle_system_type_flags.h"

void particle_system_next_particle_state_index(ps_particle_datum *particle, const particle_system_type *type_definition)
{
    unsigned int states_moving_forward = particle->states_moving_forward;
    int state_index = particle->state_index;
    int step = states_moving_forward == 0 ? -1 : 1;
    int next = (int16_t)(step + state_index);

    particle->transition_state_index = next;

    if ( next < 0 || next >= type_definition->particle_states.count )
    {
        unsigned int flags = type_definition->flags;
        int count;

        if ( (flags & (1u << _particle_system_type_particle_states_loop_bit)) != 0
          && (count = type_definition->particle_states.count, count > 0) )
        {
            if ( (flags & (1u << _particle_system_type_particle_states_loop_forward_backward_bit)) != 0 )
            {
                int bounce = state_index - step;
                if ( bounce >= 0 )
                {
                    if ( bounce > count - 1 )
                        bounce = count - 1;
                    particle->transition_state_index = bounce;
                    particle->states_moving_forward = (states_moving_forward == 0);
                }
                else
                {
                    particle->transition_state_index = 0;
                    particle->states_moving_forward = (states_moving_forward == 0);
                }
            }
            else
            {
                particle->transition_state_index = 0;
            }
        }
        else
        {
            particle->state_index = -1;
            particle->transition_state_index = -1;
        }
    }
}
