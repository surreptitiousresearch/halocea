/* change_team_for_player_in_network_game_data @0x83802A08 — finds the player slot (by player_list_index,
 * scanning the fixed 32-entry players array) and assigns its team_index. Returns whether the team
 * actually changed; returns false if the player isn't found. */

#include <stdint.h>
#include "headers/network_game_data.h"

BOOL change_team_for_player_in_network_game_data(network_game_data *data, int player_index, char team_index)
{
    network_player *player = data->players;

    for (int i = 0; player->player_list_index != player_index; i++, player++)
    {
        if (i + 1 >= 32)
            return 0;
    }

    char previous_team_index = player->team_index;
    player->team_index = team_index;
    return (uint8_t)team_index != (uint8_t)previous_team_index;
}
