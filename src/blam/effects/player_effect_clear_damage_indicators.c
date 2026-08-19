/* player_effect_clear_damage_indicators @0x83737E50 */
/* player_effect_clear_damage_indicators 0x83737E50 — reset the local player's four directional damage-indicator
 * timers to zero (a single 4-byte store over the damage_indicator_ticks[4] array). */

#include <stdint.h>
#include "headers/player_effect_globals.h"
#include "headers/blam_data_globals.h"


void player_effect_clear_damage_indicators(int16_t local_player_index)
{
    *(unsigned int *)player_effect_globals->local_player_effect_data[local_player_index].damage_indicator_ticks = 0;
}
