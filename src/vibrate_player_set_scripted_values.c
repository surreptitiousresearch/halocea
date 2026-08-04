/* vibrate_player_set_scripted_values @0x83739170 — set the scripted (script-driven) controller rumble
 * left/right motor frequencies. */

#include "headers/vibrate_global_data.h"

void vibrate_player_set_scripted_values(float left, float right)
{
    vibrate_globals->scripted_vibrate.left_frequency = left;
    vibrate_globals->scripted_vibrate.right_frequency = right;
}
