/* action_obey_individual_update @0x837DEE48 */
#include <stdint.h>
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/obey_simple_control_flags.h"

extern void action_obey_directmovement_update_facing(int actor_index, int unit_index, obey_individual_simple_control *simple_control);

void action_obey_individual_update(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, void *user_data)
{
    if ( simple_control->pause_timer > 0 )
        simple_control->pause_timer--;

    uint8_t flags = simple_control->simple_control_flags;
    if ( flags & (1u << _obey_simple_jump_bit) )
    {
        /* jump-active branch; union offset 0 aliases directmovement.facing / jump.delay_ticks. */
        if ( simple_control->___u5.directmovement.facing > 0 )
            simple_control->___u5.directmovement.facing--;
    }

    if ( (flags & (1u << _obey_simple_directmovement_bit))
      && (flags & (1u << _obey_simple_directmovement_update_continuously_bit)) )
        action_obey_directmovement_update_facing(actor_index, unit_index, simple_control);
}
