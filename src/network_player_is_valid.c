/* network_player_is_valid @0x83782C10 — a network player is valid when present, with a controller index of
 * 0 or 1 and a machine index in [0, 16). */

#include <stdint.h>
#include "headers/network_player.h"
#include "headers/network_constants.h"

uint8_t network_player_is_valid(network_player *player)
{
    if ( !player )
        return 0;
    if ( (unsigned int)player->controller_index >= MAXIMUM_NUMBER_OF_LOCAL_PLAYERS )
        return 0;
    int machine_index = player->machine_index;
    if ( machine_index < 0 )
        return 0;
    if ( machine_index >= MAXIMUM_NETWORK_MACHINE_COUNT )
        return 0;
    return 1;
}
