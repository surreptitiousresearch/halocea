/* network_game_get_random_seed @0x83756AA0 — return the network game's shared random seed.
 * 0x544 IS network_game_data.network_game_random_seed (DB types_members) read off a NULL base the compiler
 * folded in — body is one `lwz r3, 0x544` = 80600544, rA = r0. See network_game_server_get_game.c. */

#include <stdint.h>

uint32_t network_game_get_random_seed(void)
{
    return *(uint32_t *)0x544;
}
