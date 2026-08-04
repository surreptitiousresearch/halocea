/* input_abstraction_set_pitch_rate @0x83724FB0 — set the configured look pitch sensitivity for a local
 * player. DEVIATION: value typed double by the decompiler (soft-float ABI); it is a float. Player index
 * biased by -1 to match the binary's slot addressing. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"

void input_abstraction_set_pitch_rate(int16_t local_player_index, float pitch_rate)
{
    input_abstraction_globals.player_control_settings[local_player_index - 1].pitch_rate = pitch_rate;
}
