#include "headers/blam_data_globals.h"
/* network_game_set_random_seed @0x837566C0 — stamps the network random seed into a fixed low-memory address
 * (0x544), gated on the server and/or client being active. Same literal-absolute-address pattern as
 * network_game_set_number_of_games_played.c (0x548) — see that file's DEVIATION note. */


void network_game_set_random_seed(unsigned int seed)
{
    if (global_network_game_server)
        *(unsigned int *)0x544 = seed;
    if (global_network_game_client)
        *(unsigned int *)0x544 = seed;
}
