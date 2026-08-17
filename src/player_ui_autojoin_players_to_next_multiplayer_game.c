/* player_ui_autojoin_players_to_next_multiplayer_game @0x83698948 */
#include "headers/player_ui_globals.h"

void player_ui_autojoin_players_to_next_multiplayer_game(void)
{
    player_ui_globals.player_data[0].prejoined_multiplayer =
        player_ui_globals.join_players_to_next_multiplayer_game[0];
    player_ui_globals.player_data[1].prejoined_multiplayer =
        player_ui_globals.join_players_to_next_multiplayer_game[1];
}
