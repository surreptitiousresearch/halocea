#include <stdint.h>
#include "headers/blam_data_globals.h"
/* network_game_server_start_frame @ 0x83756AA8 — true only if there is no server object
 * (the caller treats a missing server as a frame-start failure) */
typedef struct network_game_server network_game_server;
/* return attested uint8_t: sole caller (main_loop_update @8368C0D4) byte-normalizes r3 (clrlwi 24) */
uint8_t network_game_server_start_frame(void)
{
    return global_network_game_server == 0;
}
