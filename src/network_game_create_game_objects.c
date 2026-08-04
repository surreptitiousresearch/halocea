/* network_game_create_game_objects @0x83782EF4 — load the map and instantiate the game world for a network
 * game session. Builds game options from the session (map name, difficulty, random seed — sourced by
 * connection role), precaches and loads the map (unloading any in-progress game first), applies the game
 * variant, and runs per-map initialization. On a server (connection 2) or film playback (3) it then spawns each
 * valid network player (machine_index 0..15, controller_index 0..1). Returns the game_objects_loaded flag.
 *
 * game_dispose_from_old_map / game_unload / main_menu_unload are independent no-arg calls (the decompiler's
 * threaded-r3 nesting). */

#include <stdint.h>
#include "headers/network_game_data.h"
#include "headers/network_player.h"
#include "headers/game_options.h"
#include "headers/game_variant.h"
#include "headers/game_connection.h"

#include "headers/network_game_server.h"
extern void game_options_new(game_options *options);
extern char *strncpy(char *destination, const char *source, unsigned int count);
extern int16_t game_connection(void);
extern uint32_t network_game_get_random_seed(void);
extern void game_precache_new_map(const char *map_name, uint8_t blocking);
extern uint8_t game_in_progress(void);
extern void game_dispose_from_old_map(void);
extern void game_unload(void);
extern void main_menu_unload(void);
extern void game_set_game_variant(game_variant *variant);
extern uint8_t game_load(game_options *options);
extern void game_initialize_for_new_map(void);
extern uint8_t network_game_spawn_player(network_player *player);
extern network_game_server *global_network_game_server_get(void);

uint8_t network_game_create_game_objects(network_game_data *game)
{
    game_options options;
    game_options_new(&options);
    strncpy(options.map_name, game->map.name, 0x7Fu);
    options.difficulty = game->difficulty_level;

    __int16 connection = game_connection();
    if (connection > 0)
    {
        if (connection <= _game_connection_network_server)
            options.random_seed = network_game_get_random_seed();
        else if (connection == _game_connection_film_playback)
            options.random_seed = game->network_game_random_seed;
    }

    game_precache_new_map(options.map_name, 1u);
    if (game_in_progress())
    {
        game_dispose_from_old_map();
        game_unload();
    }
    main_menu_unload();

    if (game->variant.game_engine_index)
        game_set_game_variant(&game->variant);

    if (game_load(&options))
    {
        game->local_data.game_objects_loaded = 1;
        game_initialize_for_new_map();
        if (game_connection() == _game_connection_network_server || game_connection() == _game_connection_film_playback)
        {
            int spawned_count = 0;
            for (network_player *player = &game->players[0]; ; ++player)
            {
                if (player == nullptr
                    || player->controller_index < 0 || player->controller_index >= 2
                    || player->machine_index < 0 || player->machine_index >= 16)
                    break;
                if (!network_game_spawn_player(player))
                {
                    game->local_data.game_objects_loaded = 0;
                    break;
                }
                if (++spawned_count >= 32)
                {
                    global_network_game_server_get();
                    return game->local_data.game_objects_loaded;
                }
            }
            global_network_game_server_get();
        }
    }

    return game->local_data.game_objects_loaded;
}
