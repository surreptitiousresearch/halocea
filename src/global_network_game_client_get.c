#include "headers/blam_data_globals.h"
/* global_network_game_client_get @ 0x83756898 */
typedef struct network_game_client network_game_client;
network_game_client *global_network_game_client_get(void)
{
    return global_network_game_client;
}
