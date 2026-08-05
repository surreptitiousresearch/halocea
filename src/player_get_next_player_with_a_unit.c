/* player_get_next_player_with_a_unit @0x837C3330 — find the next player (after old_player_index, cycling)
 * that has a living unit and, when match_team is set, is on the same team as player_index. Returns that
 * player's index; if no eligible player has an index past old_player_index it wraps to the first eligible
 * one; if none qualify at all it returns old_player_index. Excludes player_index itself and players with no
 * unit (player +52). Team is player +32; the iterator's team field is v8[8] (player +32).
 * Reproduces the decompiler's single-pass loop: hold the first eligible player, and accept the first
 * subsequent eligible player whose index exceeds old_player_index. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/blam_data_globals.h"


int player_get_next_player_with_a_unit(int player_index, int old_player_index, uint8_t match_team)
{
    int team = match_team
        ? DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->team_index
        : -1;

    int index = -1;
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return old_player_index;

    while ( 1 )
    {
        int eligible = !(iterator.index == player_index || player->unit_index == -1 || (match_team && player->team_index != team));
        if ( eligible )
        {
            if ( index != -1 )
            {
                if ( (int16_t)iterator.index > (int)(uint16_t)old_player_index )
                {
                    index = iterator.index;
                    break;
                }
            }
            else
            {
                index = iterator.index;
            }
        }
        player = data_iterator_next(&iterator);
        if ( !player )
            break;
    }

    return index == -1 ? old_player_index : index;
}
