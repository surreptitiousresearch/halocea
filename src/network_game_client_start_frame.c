/* network_game_client_start_frame @ 0x837568E8 — if a networking teardown was requested,
 * drop to local play: unload the menu, end the net game, clear client/server objects, and
 * return to the main menu. Returns 1 when a teardown happened, 0 otherwise. */
#include <stdint.h>
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"
typedef struct network_game_client network_game_client;
typedef struct network_game_server network_game_server;
#include "headers/network_game_data.h"
extern void game_connection_set(int16_t new_connection);
extern void main_menu_unload(void);
extern void network_game_end_and_load_ui(network_game_data *game);
extern void main_goto_main_menu(void);

uint8_t network_game_client_start_frame(void)
{
    if ( want_to_teardown_networking != 1 )
        return 0;

    game_connection_set(_game_connection_local);
    main_menu_unload();
    network_game_end_and_load_ui(0);
    want_to_teardown_networking = 0;
    if ( global_network_game_client )
        global_network_game_client = 0;
    if ( global_network_game_server )
    {
        global_network_game_server = 0;
        quickstart_network_game_active = 0;
    }
    main_goto_main_menu();
    return 1;
}
