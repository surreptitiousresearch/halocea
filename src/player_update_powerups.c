/* player_update_powerups @0x836AC728 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/unit_flags.h"

/* Tick down the player's two powerup timers.  When the first timer (index 0)
 * reaches expiry (was 1, now 0) the player's unit loses its active powerup flag
 * (unit.flags bit 0x10).  DEVIATION: decompiler emitted raw pointer arithmetic;
 * offsets resolve to player_datum.powerup_durations[i] (0x68),
 * player_datum.unit_index (0x34) and unit_datum.unit.flags (0x204). */
void player_update_powerups(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    for ( int i = 0; i < 2; i++ )
    {
        int16_t duration = player->powerup_durations[i];
        if ( duration > 0 )
        {
            player->powerup_durations[i] = duration - 1;
            if ( duration == 1 && i == 0 )
            {
                unit_datum *unit_object = (unit_datum *)DATUM_GET(
                    object_header_data, object_header_datum,
                    player->unit_index)->datum;
                unit_object->unit.flags &= ~(1u << _unit_active_camouflaged_bit);
            }
        }
    }
}
