/* network_game_get_random_seed @0x83756AA0 — return the network game's shared random seed.
 * The seed is held in a fixed low-memory scratch location (absolute address 0x544); the body is a
 * single load (lwz r3, 0x544). */

#include <stdint.h>

uint32_t network_game_get_random_seed(void)
{
    return *(uint32_t *)0x544;
}
