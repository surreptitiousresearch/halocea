/* particle_system_next_type_state_index @0x8373A580 — pick the next emitter state for a particle system
 * type. Steps the type's state index forward or backward (per states_moving_forward); on running off the
 * end of the type_states block it bounces (type_states_loop_forward_backward: clamp and reverse), wraps
 * to 0 (type_states_loop only), or deactivates the type. Looping only continues while the system has a
 * host object. */

#include "headers/particle_system_datum.h"
#include "headers/particle_type.h"
#include "headers/particle_system_type.h"
#include "headers/particle_system_type_flags.h"

void particle_system_next_type_state_index(const particle_system_datum *system, particle_type *type,
                                           const particle_system_type *type_definition)
{
    unsigned int states_moving_forward = type->states_moving_forward;
    int state_index = type->state_index;
    int step = states_moving_forward == 0 ? -1 : 1;
    int next = (__int16)(step + state_index);

    type->transition_state_index = next;

    if ( next < 0 || next >= type_definition->type_states.count )
    {
        unsigned int flags = type_definition->flags;
        int count;

        if ( (flags & (1u << _particle_system_type_type_states_loop_bit)) == 0 || system->object_index == -1
          || (count = type_definition->type_states.count, count <= 0) )
        {
            type->state_index = -1;
            type->transition_state_index = -1;
        }
        else if ( (flags & (1u << _particle_system_type_type_states_loop_forward_backward_bit)) != 0 )
        {
            int bounce = state_index - step;
            if ( bounce >= 0 )
            {
                if ( bounce > count - 1 )
                    bounce = count - 1;
                type->transition_state_index = bounce;
                type->states_moving_forward = (states_moving_forward == 0);
            }
            else
            {
                type->transition_state_index = 0;
                type->states_moving_forward = (states_moving_forward == 0);
            }
        }
        else
        {
            type->transition_state_index = 0;
        }
    }
}
