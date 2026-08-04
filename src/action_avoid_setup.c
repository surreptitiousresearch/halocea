/* action_avoid_setup @0x83821278 — initializes the "avoid" action's scratch state (zeroes it) and always
 * reports success. `actor_index` is unused (confirmed via disasm: only r4/state_data is touched). */

#include <stdint.h>
#include "headers/avoid_state_data.h"

uint8_t action_avoid_setup(int actor_index, avoid_state_data *state_data)
{
    state_data->pad = 0;
    return 1;
}
