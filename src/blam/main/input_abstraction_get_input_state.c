/* input_abstraction_get_input_state @0x83724B20 — return a pointer to a local player's abstracted game input
 * state. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/blam_data_globals.h"


game_input_state *input_abstraction_get_input_state(int16_t local_player_index)
{
    return &input_abstraction_globals.input_state[local_player_index];
}
