#include "headers/blam_data_globals.h"
/* dispose_global_network_game_server @ 0x83756870 */


void dispose_global_network_game_server(void)
{
    if ( global_network_game_server )
    {
        global_network_game_server = 0;
        quickstart_network_game_active = 0;
    }
}
