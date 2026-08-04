#include <stdint.h>
#include "headers/blam_data_globals.h"
/* network_game_is_active @ 0x83756660 — true if either a network client or server
 * object is allocated. */


uint8_t network_game_is_active(void)
{
    if ( global_network_game_client )
        return 1;
    if ( global_network_game_server )
        return 1;
    return 0;
}
