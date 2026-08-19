#include <stdint.h>
#include "headers/blam_data_globals.h"
/* create_global_network_game_client @0x837568A8 — on this build the network game client is never
 * instantiated: the global is cleared and creation reports failure (returns 0). */


uint8_t create_global_network_game_client(void)
{
    global_network_game_client = nullptr;
    return 0;
}
