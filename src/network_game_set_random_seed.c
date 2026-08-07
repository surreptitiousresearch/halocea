#include "headers/blam_data_globals.h"
/* network_game_set_random_seed @0x837566C0 — stamps network_game_data.network_game_random_seed (0x544,
 * DB types_members) over a NULL base the compiler folded in, gated on the server and/or client being
 * active. Both stores are 90600544 (rA = r0). Mechanism: see network_game_server_get_game.c. */


void network_game_set_random_seed(unsigned int seed)
{
    if (global_network_game_server)
        *(unsigned int *)0x544 = seed;
    if (global_network_game_client)
        *(unsigned int *)0x544 = seed;
}
