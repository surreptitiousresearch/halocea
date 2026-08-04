#include <stdint.h>
#include "headers/network_game_client.h"
#include "headers/blam_data_globals.h"


int16_t network_game_client_get_local_machine_index(void)
{
    return (global_network_game_client != 0) - 1;
}
