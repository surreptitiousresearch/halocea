/* action_fight_setup @0x83825F90 — initializes the "fight" action's scratch state and always reports
 * success. `actor_index` is unused. */

#include <stdint.h>
#include "headers/fight_state_data.h"

uint8_t action_fight_setup(int actor_index, fight_state_data *state_data)
{
    state_data->firing_position_timer = 0;
    state_data->pad = 0;
    return 1;
}
