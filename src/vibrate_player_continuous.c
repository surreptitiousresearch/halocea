/* vibrate_player_continuous @0x83739370 — set a local player's continuous (sustained) controller
 * rumble levels.
 *
 * DEVIATION: the strengths arrive as doubles via the soft-float ABI; the stored fields are floats. */

#include <stdint.h>
#include "headers/vibrate_global_data.h"

void vibrate_player_continuous(int16_t local_player_index, float low_frequency_strength, float high_frequency_strength)
{
    player_vibrate_datum *vibrate = &vibrate_globals->player_vibrate_data[local_player_index];
    vibrate->left_frequency_continuous = low_frequency_strength;
    vibrate->right_frequency_continuous = high_frequency_strength;
}
