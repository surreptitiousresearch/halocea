#include "headers/blam_data_globals.h"
/* dispose_global_network_game_client @ 0x837568C0 */


void dispose_global_network_game_client(void)
{
    want_to_teardown_networking = 0;
    if ( global_network_game_client )
        global_network_game_client = 0;
}
