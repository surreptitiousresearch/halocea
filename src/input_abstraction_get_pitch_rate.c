/* input_abstraction_get_pitch_rate @0x83724FE0 — configured look pitch sensitivity for a local player.
 * DEVIATION: float return routed through a double (soft-float ABI); the real return is the float. Player
 * index biased by -1 to match the binary's slot addressing. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"

float input_abstraction_get_pitch_rate(int16_t local_player_index)
{
    return input_abstraction_globals.player_control_settings[local_player_index - 1].pitch_rate;
}
