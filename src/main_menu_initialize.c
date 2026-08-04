/* main_menu_initialize @0x83692360 — main-menu widget initialize handler: clear any pending multiplayer
 * joins/variant, tear down the network client/server, stop accepting remote connections, reset the
 * connection mode and single-player spawn state, end any profile editing, and start the menu music. Returns
 * 1 (handled).
 *
 * Deviation: the player_ui_* / dispose_* callees are void no-arg; the decompiler chained them through r3
 * (with the unused widget/event/widget_deleted handler params). Unchained. The widget_instance parameter is
 * unused here, so it is left opaque. */

#include <stdint.h>
#include "headers/event_record.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

typedef struct widget_instance widget_instance;

extern void player_ui_clear_multiplayer_joins(void);
extern void player_ui_clear_multiplayer_variant(void);
extern void dispose_global_network_game_client(void);
extern void dispose_global_network_game_server(void);
extern void network_game_accept_remote_connections(uint8_t accept);
extern void game_connection_set(int16_t new_connection);
extern void player_ui_end_editing_profile(void);
extern uint8_t ui_main_menu_music_active(void);
extern void ui_start_main_menu_music(void);

uint8_t main_menu_initialize(widget_instance *widget, event_record *event, unsigned char *widget_deleted)
{
    player_ui_clear_multiplayer_joins();
    player_ui_clear_multiplayer_variant();
    dispose_global_network_game_client();
    dispose_global_network_game_server();
    network_game_accept_remote_connections(0);
    game_connection_set(_game_connection_local);
    player_spawn_count = 1;
    coop_mode_selected = 0;
    player_ui_end_editing_profile();
    if ( !ui_main_menu_music_active() )
        ui_start_main_menu_music();
    return 1;
}
