/* main_loop_term @ 0x8368AE90 — standalone loop teardown */

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
extern void game_dispose(void); /* attested void */
extern void debug_keys_dispose(void);
extern int net_graph_dispose(void);
extern void console_dispose(void);
extern void hud_chat_dispose(void);

void main_loop_term(void) /* attested void: 0/2 callers consume r3 */
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
            save_film_end_recording(); /* attested void */
            save_film_end_playback();
            break;
    }
    game_dispose_from_old_map();
    game_dispose(); /* attested void */
    debug_keys_dispose();
    net_graph_dispose(); /* returned status unused */
    console_dispose();
    hud_chat_dispose(); /* attested void(void): r3-thread removed */
}
