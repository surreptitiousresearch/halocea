/* player_effect_get_damage_indicators @0x83737DA0 */
/* player_effect_get_damage_indicators 0x83737DA0 — copy the local player's four directional damage-indicator
 * timers into the caller's buffer, then advance each non-zero timer by the elapsed game time (saturating at
 * 255). The timers count up toward 255 (fully faded). */

#include <stdint.h>
#include "headers/player_effect_globals.h"
#include "headers/blam_data_globals.h"

extern int16_t game_time_get_elapsed(void);

void player_effect_get_damage_indicators(int16_t local_player_index, uint8_t *damage_indicators)
{
    player_effect_datum *effect = &player_effect_globals->local_player_effect_data[local_player_index];

    damage_indicators[0] = effect->damage_indicator_ticks[0];
    damage_indicators[1] = effect->damage_indicator_ticks[1];
    damage_indicators[2] = effect->damage_indicator_ticks[2];
    damage_indicators[3] = effect->damage_indicator_ticks[3];

    for ( int16_t i = 0; i < 4; ++i )
    {
        if ( effect->damage_indicator_ticks[i] )
        {
            if ( game_time_get_elapsed() + effect->damage_indicator_ticks[i] >= 255 )
                effect->damage_indicator_ticks[i] = 0xFF;
            else
                effect->damage_indicator_ticks[i] = game_time_get_elapsed() + effect->damage_indicator_ticks[i];
        }
    }
}
