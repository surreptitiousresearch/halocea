/* player_ui_fast_setup_network_server @0x83698D50 — fast path to host a multiplayer game: tears down
 * any existing server/client, loads the requested map and variant, opens the connected-pregame screen,
 * then creates a fresh server (and client). If anything fails it disposes and returns to the main menu.
 *
 * The leading ui_widgets_close_all / dispose_* calls are independent no-arg calls (threaded-r3 artifact). */

#include <stdint.h>
#include "headers/game_variant.h"
#include "headers/player_ui_globals.h"
#include "headers/game_connection.h"

extern void ui_widgets_close_all(void); /* attested void */
extern void dispose_global_network_game_server(void);
extern void dispose_global_network_game_client(void);
extern void game_connection_set(int16_t new_connection);
extern void main_set_multiplayer_map_name(const char *name);
extern uint8_t game_engine_get_variant_by_name(const char *name, game_variant *variant);
extern void *memcpy(void *dst, const void *src, unsigned int size);
typedef struct widget_instance widget_instance;
extern widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent, __int16 local_player_index, int invoking_widget_tag, int focused_child_parent_widget_tag, __int16 focused_child_index);
extern void game_engine_playlist_initialize(unsigned __int8 force_begin_playlist);
extern void network_game_accept_remote_connections(uint8_t accept);
extern uint8_t create_global_network_game_server(void);
extern uint8_t create_global_network_game_client(void);
extern void game_engine_playlist_begin(void);
extern void main_goto_main_menu(void);

void player_ui_fast_setup_network_server(char *map_name, char *variant_name, uint8_t accept_remote_connections)
{
    char succeeded = 0;

    ui_widgets_close_all();
    dispose_global_network_game_server();
    dispose_global_network_game_client();
    game_connection_set(_game_connection_local);
    main_set_multiplayer_map_name(map_name);

    game_variant variant;
    game_engine_get_variant_by_name(variant_name, &variant);
    memcpy(&player_ui_globals.variant, &variant, sizeof(player_ui_globals.variant));
    player_ui_globals.user_selected_variant = 1;

    if (ui_widget_load_by_name_or_tag(
            "ui\\shell\\main_menu\\multiplayer_type_select\\connected\\pregame\\connected_pregame_screen",
            -1, nullptr, -1, -1, -1, -1))
    {
        game_engine_playlist_initialize(0);
        network_game_accept_remote_connections(accept_remote_connections);
        if (create_global_network_game_server() && create_global_network_game_client())
        {
            game_engine_playlist_begin();
            game_connection_set(_game_connection_network_server);
            succeeded = 1;
        }
        else
        {
            dispose_global_network_game_server();
            dispose_global_network_game_client();
            accept_remote_connections = 0; /* server failed: stop accepting */
            network_game_accept_remote_connections(accept_remote_connections);
        }
    }

    if (!succeeded)
        main_goto_main_menu();
}
