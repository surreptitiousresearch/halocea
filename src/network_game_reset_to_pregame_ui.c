/* network_game_reset_to_pregame_ui @0x83736718 — close all UI and open the pregame/postgame lobby screen
 * appropriate to the session: splitscreen (quickstart pregame wrapper vs map-select postgame wrapper) or
 * system-link/connected (server: map-select postgame wrapper — the compiled code calls
 * global_network_game_server_get a second time and discards it, reproduced; client: pregame screen). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/network_game_server.h"

extern void ui_widgets_close_all(void);
extern uint8_t network_game_is_splitscreen_local(void);
extern uint8_t network_game_is_quickstart_local(void);
extern network_game_server *global_network_game_server_get(void);
extern widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent, __int16 local_player_index, int invoking_widget_tag, int focused_child_parent_widget_tag, __int16 focused_child_index);

widget_instance *network_game_reset_to_pregame_ui(void)
{
    ui_widgets_close_all();

    if ( network_game_is_splitscreen_local() )
    {
        if ( network_game_is_quickstart_local() )
            return ui_widget_load_by_name_or_tag(
                "ui\\shell\\main_menu\\multiplayer_type_select\\split_screen\\pregame\\splitscreen_pregame_wrapper_normal",
                -1, 0, -1, -1, -1, -1);
        return ui_widget_load_by_name_or_tag(
            "ui\\shell\\main_menu\\multiplayer_type_select\\split_screen\\splitscreen_map_select_postgame_wrapper",
            -1, 0, -1, -1, -1, -1);
    }

    const char *screen_name;
    if ( global_network_game_server_get() )
    {
        global_network_game_server_get();   /* second call's result discarded, as compiled */
        screen_name = "ui\\shell\\main_menu\\multiplayer_type_select\\connected\\connected_map_select_postgame_wrapper";
    }
    else
    {
        screen_name = "ui\\shell\\main_menu\\multiplayer_type_select\\connected\\pregame\\connected_pregame_screen";
    }
    return ui_widget_load_by_name_or_tag(screen_name, -1, 0, -1, -1, -1, -1);
}
