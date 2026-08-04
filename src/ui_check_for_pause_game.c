/* ui_check_for_pause_game @ 0x83736C40 — detect a start-button press from a local player
 * and, if pausing is allowed for the current context (solo / split-screen / network), spawn
 * the appropriate pause-menu widget. Returns 1 if a pause widget was loaded this frame. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/input_globals.h"
#include "headers/game_connection.h"
#include "headers/gamepad_button.h"

#include "headers/gamepad_state.h"
#include "headers/network_game_client.h"
#include "headers/widget_instance.h"
#include "headers/widget_instance.h"
extern uint8_t network_game_is_active(void);
extern uint8_t game_in_progress(void);
extern uint8_t cinematic_in_progress(void);
extern int16_t game_connection(void);
extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern uint8_t local_player_exists(int local_player_index);
extern uint8_t game_time_get_paused(void);
extern uint8_t game_engine_allow_pause(void);
extern uint8_t ui_widgets_active(void);
extern network_game_client *global_network_game_client_get(void);
extern int16_t local_player_get_next(int16_t local_player_index);
extern widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent, __int16 local_player_index, int invoking_widget_tag, int focused_child_parent_widget_tag, __int16 focused_child_index);

extern gamepad_state *input_get_gamepad_state(int16_t gamepad_index);

int ui_check_for_pause_game(void)
{
    char start_pressed = 0;
    unsigned char networked = network_game_is_active();
    __int16 pressing_controller = -1;
    int pause_loaded = 0;
    int controller;

    if ( game_in_progress()
      && !cinematic_in_progress()
      && game_connection() != _game_connection_film_playback
      && !we_are_at_the_main_menu
      && !widget_globals.pause_disabled_ticks )
    {
        for ( controller = 0; controller < 4; ++controller )
        {
            if ( input_has_gamepad(controller)
              && local_player_exists(controller)
              && input_get_gamepad_state(controller)->buttons[_gamepad_binary_button_start] == 1 )
            {
                start_pressed = 1;
                pressing_controller = controller;
                break;
            }
        }
    }

    if ( start_pressed )
    {
        int only_presser_is_solo = 1;
        __int16 local_player_count = 0;
        __int16 matched_controller = -1;
        __int16 player;

        for ( player = local_player_get_next(-1); player != -1; player = local_player_get_next(player) )
        {
            if ( player == pressing_controller )
            {
                matched_controller = pressing_controller;
                only_presser_is_solo &= (local_player_count == 0);  /* true only if first listed */
            }
            ++local_player_count;
        }

        if ( networked )
        {
            if ( game_engine_allow_pause()
              && matched_controller == pressing_controller
              && !widget_globals.active_widgets[pressing_controller] )
            {
                const char *widget_name = 0;
                global_network_game_client_get();
                if ( (unsigned int)(local_player_count - 1) <= 3 )
                {
                    if ( local_player_count == 2 )
                        widget_name = "ui\\shell\\multiplayer_game\\pause_game\\2p_pause_game";
                    else if ( local_player_count == 3 )
                        widget_name = (unsigned char)only_presser_is_solo == 1
                            ? "ui\\shell\\multiplayer_game\\pause_game\\2p_pause_game"
                            : "ui\\shell\\multiplayer_game\\pause_game\\4p_pause_game";
                    else if ( local_player_count == 1 )
                        widget_name = "ui\\shell\\multiplayer_game\\pause_game\\1p_pause_game";
                    else
                        widget_name = "ui\\shell\\multiplayer_game\\pause_game\\4p_pause_game";

                    if ( widget_name )
                    {
                        ui_widget_load_by_name_or_tag(widget_name, -1, 0, pressing_controller, -1, -1, -1);
                        pause_loaded = 1;
                    }
                }
            }
        }
        else
        {
            /* solo / split-screen pause */
            const char *widget_name = 0;
            int load_simple_pause = 0;

            if ( local_player_count < 2 )
            {
                load_simple_pause = (widget_globals.active_widgets[pressing_controller] == 0);
            }
            else if ( local_player_count == 2 )
            {
                if ( !widget_globals.active_widgets[pressing_controller] && !game_time_get_paused() )
                    widget_name = "ui\\shell\\solo_game\\pause_game\\pause_game_split_screen";
            }
            else
            {
                load_simple_pause = (ui_widgets_active() == 0);
            }

            if ( !widget_name && load_simple_pause )
                widget_name = "ui\\shell\\solo_game\\pause_game\\pause_game";

            if ( widget_name )
            {
                ui_widget_load_by_name_or_tag(widget_name, -1, 0, pressing_controller, -1, -1, -1);
                pause_loaded = 1;
            }
        }
    }

    /* count the pause-inhibit timer down, clamping at zero */
    if ( widget_globals.pause_disabled_ticks > 0 )
        --widget_globals.pause_disabled_ticks;

    return pause_loaded;
}
