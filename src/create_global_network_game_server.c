#include <stdint.h>
#include "headers/blam_data_globals.h"
/* create_global_network_game_server @0x83756864 — on this build the network game server is never
 * instantiated: the global is cleared and creation reports failure (returns 0). */


uint8_t create_global_network_game_server(void)
{
    global_network_game_server = nullptr;
    return 0;
}
