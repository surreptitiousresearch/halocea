/* input_abstraction_get_yaw_rate @0x83724FB8 — configured look yaw sensitivity for a local player.
 * DEVIATION: the decompiler routed the float return through a double (soft-float ABI artifact); the real
 * return is just the float. The player index is biased by -1 to match the binary's slot addressing. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"

float input_abstraction_get_yaw_rate(int16_t local_player_index)
{
    return input_abstraction_globals.player_control_settings[local_player_index - 1].yaw_rate;
}
