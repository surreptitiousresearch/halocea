/* king_get_score_string @0x8382CF30 — formats an individual player's king-of-the-hill time-with-ball
 * (player_datum.statistics.multiplayer_statistics.king_statistics.time_on_hill, byte +196) as a
 * ticks-based time string. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern void ticks_to_unicode_time_string(int ticks, unsigned int count, unsigned __int16 *buffer);

uint16_t * king_get_score_string(int player_index, uint16_t *buffer)
{
    __int16 time_on_hill = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->statistics.multiplayer_statistics.king_statistics.time_on_hill;
    ticks_to_unicode_time_string(time_on_hill, 256, buffer);
    return buffer;
}
