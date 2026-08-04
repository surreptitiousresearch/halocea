/* main_exit @0x8368A770 — tear down the standalone main loop: free the map/ban/mapcycle lists, dispose the
 * active network session (client / client+server / film recording+playback depending on connection mode),
 * then dispose game, debug keys, net graph, console and chat HUD.
 *
 * Deviation: every callee is void no-arg; the decompiler chains them through r3 and the database types
 * main_exit as int. All return values are ABI residue, so this is written as void with the calls unchained. */

#include "headers/main_globals.h"
#include "headers/game_connection.h"

extern void map_list_free_known_list(void);
extern void banlist_free(void);
extern void mapcycle_free(void);
extern void dispose_global_network_game_client(void);
extern void dispose_global_network_game_server(void);
extern void save_film_end_recording(void);
extern void save_film_end_playback(void);
extern void game_dispose_from_old_map(void);
extern void game_dispose(void);
extern void debug_keys_dispose(void);
extern int net_graph_dispose(void);
extern void console_dispose(void);
extern void hud_chat_dispose(void);

void main_exit(void)
{
    map_list_free_known_list();
    banlist_free();
    mapcycle_free();

    switch ( main_globals.connection )
    {
        case _game_connection_network_client:
            dispose_global_network_game_client();
            break;
        case _game_connection_network_server:
            dispose_global_network_game_client();
            dispose_global_network_game_server();
            break;
        case _game_connection_film_playback:
            save_film_end_recording();
            save_film_end_playback();
            break;
    }

    game_dispose_from_old_map();
    game_dispose();
    debug_keys_dispose();
    net_graph_dispose();
    console_dispose();
    hud_chat_dispose();
}
