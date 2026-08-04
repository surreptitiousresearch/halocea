/* network_game_server_list_initialize @0x836920C8 — menu handler: drop any existing global network
 * game objects, then create a fresh network game client and switch the game connection to the
 * network-client mode (1). Reports failure (0) if the client could not be created. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_connection.h"

extern void dispose_global_network_game_client(void);
extern void dispose_global_network_game_server(void);
extern void player_ui_clear_multiplayer_variant(void);
extern uint8_t create_global_network_game_client(void);
extern void game_connection_set(int16_t new_connection);

uint8_t network_game_server_list_initialize(widget_instance *widget, event_record *event,
        unsigned __int8 *widget_deleted)
{
    dispose_global_network_game_client();
    dispose_global_network_game_server();
    player_ui_clear_multiplayer_variant();

    if ( !create_global_network_game_client() )
        return 0;

    game_connection_set(_game_connection_network_client);
    return 1;
}
