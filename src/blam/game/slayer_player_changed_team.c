/* slayer_player_changed_team @0x83815AA0 */
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/slayer_globals.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);

void slayer_player_changed_team(int player_index, uint8_t new_team)
{
    player_datum *player = datum_try_and_get(player_data, player_index);

    if ( player )
    {
        player->multiplayer.special = -1;
        slayer_globals.individual_score[player_index] = 0;
    }
}
