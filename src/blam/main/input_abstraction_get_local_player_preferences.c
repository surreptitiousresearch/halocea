/* input_abstraction_get_local_player_preferences @0x83724A90 — copy of a local player's control
 * preferences. DEVIATION: the binary copies the struct via a pre-incremented 9-word loop (lwzu/stwu
 * anchored one word back); reproduced as a plain struct copy of equal effect.
 * ATTEST FIX 2026-07-28: source element is player_control_settings[local_player_index] (disasm
 * 0x83724A98..0x83724AB0 computes base + index*36 exactly); prior "- 1" index was a recon bug. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"

void input_abstraction_get_local_player_preferences(int16_t local_player_index, game_input_preferences *preferences)
{
    *preferences = input_abstraction_globals.player_control_settings[local_player_index];
}
