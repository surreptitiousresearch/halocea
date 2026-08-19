/* network_game_invalidate_player @0x837827F0 — resets a network_player slot back to "unused". */

#include "headers/network_player.h"

void network_game_invalidate_player(network_player *player)
{
    player->machine_index = -1;
    player->controller_index = -1;
    player->team_index = -1;
    player->player_list_index = -1;
    player->name[0] = 0;
    player->primary_color_index = -1;
    player->icon_index = -1;
}
