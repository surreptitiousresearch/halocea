#include "headers/blam_data_globals.h"
/* network_game_set_number_of_games_played @0x83756690 — stamps network_game_data.number_of_games_played
 * (offset 0x548, DB types_members), gated on the server and/or client being active.
 *
 * DEVIATION: both stores encode as 90600548 — `stw r3, 0x548(r0)`, rA = r0. The base is not
 * global_network_game_server/global_network_game_client (those only feed the null checks) but the NULL
 * that the network_game_server_get_game() stub folds in — see network_game_server_get_game.c. */


void network_game_set_number_of_games_played(int number_of_games_played)
{
    if (global_network_game_server)
        *(int *)0x548 = number_of_games_played;
    if (global_network_game_client)
        *(int *)0x548 = number_of_games_played;
}
