/* input_abstraction_set_yaw_rate @0x83724F68 — set the configured look yaw sensitivity for a local player.
 * DEVIATION: the decompiler typed the value as double (soft-float ABI); it is a float. The player index is
 * biased by -1 to match the binary's slot addressing. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"

void input_abstraction_set_yaw_rate(int16_t local_player_index, float yaw_rate)
{
    input_abstraction_globals.player_control_settings[local_player_index - 1].yaw_rate = yaw_rate;
}
