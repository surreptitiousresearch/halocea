/* players_are_all_dead @ 0x836A8318 — true when every player is currently dead. */

#include <stdint.h>
#include "headers/players_global_data.h"

uint8_t players_are_all_dead(void)
{
    return players_globals->all_dead;
}
