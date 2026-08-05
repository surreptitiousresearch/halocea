/* split_screen_game_initialize @0x836922A0 — menu handler: start a local split-screen game. Refuse
 * remote connections, create a server (initialize + begin the game-engine playlist) if none exists,
 * then ensure a client exists; unwind on failure. Returns non-zero on success. (Decompiler-threaded
 * call arguments are register-leftover noise; dispose_* callees are void(void).) */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/network_game_server.h"
#include "headers/network_game_client.h"
#include "headers/game_connection.h"

extern void network_game_accept_remote_connections(uint8_t accept);
extern network_game_server *global_network_game_server_get(void);
extern network_game_client *global_network_game_client_get(void);
extern int game_engine_playlist_initialize(uint8_t force_begin_playlist);
extern int game_engine_playlist_begin(void);
extern uint8_t create_global_network_game_server(void);
extern uint8_t create_global_network_game_client(void);
extern void game_connection_set(int16_t new_connection);
extern void dispose_global_network_game_server(void);
extern void dispose_global_network_game_client(void);
extern void player_ui_clear_multiplayer_variant(void);

uint8_t split_screen_game_initialize(widget_instance *widget, event_record *event,
        uint8_t *widget_deleted)
{
    uint8_t success = 1;

    network_game_accept_remote_connections(0);

    if ( !global_network_game_server_get() )
    {
        game_engine_playlist_initialize(0);
        success = create_global_network_game_server();
        if ( success == 1 )
        {
            game_engine_playlist_begin();
            game_connection_set(_game_connection_network_server);
        }
    }

    if ( success )
    {
        if ( !global_network_game_client_get() )
            success = create_global_network_game_client();
    }

    if ( !success )
    {
        dispose_global_network_game_server();
        dispose_global_network_game_client();
        player_ui_clear_multiplayer_variant();
    }

    return success;
}
