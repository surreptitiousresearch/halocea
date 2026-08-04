/* network_game_reset_for_next_round @0x83782A60 — transition to the next round. If asked to unload game
 * objects and objects are currently loaded, ends the round and returns to the UI, then tells the network
 * layer (server or client) to disconnect its game_connection and stop the game clock. Otherwise, just clears
 * the "objects loaded" bookkeeping and stops the clock. */

#include <stdint.h>
#include "headers/network_game_data.h"
#include "headers/game_connection.h"

typedef struct network_game_server network_game_server;
typedef struct network_game_client network_game_client;

extern void network_game_end_and_load_ui(network_game_data *game);
extern network_game_server *global_network_game_server_get(void);
extern network_game_client *global_network_game_client_get(void);
extern void game_connection_set(int16_t new_connection);
extern void game_time_end(void);

void network_game_reset_for_next_round(network_game_data *game, uint8_t unload_game_objects)
{
    if ( unload_game_objects && game->local_data.game_objects_loaded )
    {
        network_game_end_and_load_ui(game);

        if ( global_network_game_server_get() )
        {
            game_connection_set(_game_connection_network_server);
            game_time_end();
            return;
        }
        if ( global_network_game_client_get() )
        {
            game_connection_set(_game_connection_network_client);
            game_time_end();
            return;
        }
    }
    else
    {
        game->local_data.game_objects_loaded = 0;
        game->local_data.pad3[0] = 0;
        game->local_data.pad3[1] = 0;
        game->local_data.pad3[2] = 0;
    }

    game_time_end();
}
