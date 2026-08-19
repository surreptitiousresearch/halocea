#include "headers/blam_data_globals.h"
/* global_network_game_server_get @ 0x83756848 */
typedef struct network_game_server network_game_server;
network_game_server *global_network_game_server_get(void)
{
    return global_network_game_server;
}
