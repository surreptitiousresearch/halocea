/* player_telefrag_effect_stop @0x837378B0 — stop the controller vibration started by a player's
 * telefrag effect, when that player slot maps to a valid local player. Player records are stride 512. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern void vibrate_player_continuous(int16_t local_player_index, float low_frequency_strength, float high_frequency_strength);

void player_telefrag_effect_stop(int player_index)
{
    short local_player_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->local_player_index;
    if ( local_player_index != -1 )
        vibrate_player_continuous(local_player_index, 0.0f, 0.0f);
}
