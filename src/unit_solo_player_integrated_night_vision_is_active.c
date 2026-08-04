/* unit_solo_player_integrated_night_vision_is_active @0x836D28E0 — in single-player (exactly one local
 * player), return whether that player's unit has its integrated night-vision active; otherwise return false. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern int16_t local_player_count(void);
extern int16_t local_player_get_next(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern uint8_t unit_integrated_night_vision_is_active(int unit_index);

uint8_t unit_solo_player_integrated_night_vision_is_active(void)
{
    if ( local_player_count() != 1 )
        return 0;
    int player_index = local_player_get_player_index(local_player_get_next(-1));
    if ( player_index == -1 )
        return 0;
    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
    if ( unit_index == -1 )
        return 0;
    return unit_integrated_night_vision_is_active(unit_index);
}
