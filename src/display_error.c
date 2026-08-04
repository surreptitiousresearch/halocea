/* display_error @ 0x837367E8 — bring up an error dialog. During a cinematic the error is
 * deferred (stored per local player) until the cinematic ends. Otherwise it picks the right
 * error widget for the local player count (full / half / quarter screen, modal or not),
 * replaces any current root widget, fills in the error string index (clamped 0..NUMBER_OF_ERROR_CODES-1),
 * marks the widget as an error dialog and optionally pauses the game / sound. The two controller-unplugged
 * codes get an immediate auto-close. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/game_connection.h"
#include "headers/error_type.h"

extern uint8_t cinematic_in_progress(void);
extern int16_t local_player_get_next(int16_t local_player_index);
extern int16_t game_connection(void);
extern uint8_t game_time_get_paused(void);
extern void game_time_set_paused(uint8_t paused);
extern void sound_pause(uint8_t paused);
extern void ui_start_main_menu_music(void);
extern void main_defer_map_map_change(void);
extern widget_instance *ui_widget_load_by_name_or_tag(const char *name, int tag_index, widget_instance *parent, __int16 local_player_index, int invoking_widget_tag, int focused_child_parent_widget_tag, __int16 focused_child_index);

void display_error(int16_t error_code, int16_t local_player_index,
                   uint8_t modal, uint8_t pause_game_time)
{
    int target_is_first = 1;
    __int16 local_player_count;
    __int16 matched_controller;
    const char *widget_name;

    if ( cinematic_in_progress() )
    {
        int slot = (local_player_index == -1) ? 0 : local_player_index;
        if ( (unsigned __int16)widget_globals.deferred_cinematic_error[slot].error_code == 0xFFFF )
        {
            widget_globals.deferred_cinematic_error[slot].error_code = error_code;
            widget_globals.deferred_cinematic_error[slot].modal = modal;
            /* note: cinematic variant has no pause_game_time field (4-byte record);
             * the original wrote past it but only error_code+modal are ever read back */
        }
        return;
    }

    if ( local_player_index == -1 )
    {
        local_player_count = 0;
        matched_controller = -1;
    }
    else
    {
        __int16 player;
        local_player_count = 0;
        matched_controller = -1;
        for ( player = local_player_get_next(-1); player != -1; player = local_player_get_next(player) )
        {
            if ( player == local_player_index )
            {
                matched_controller = local_player_index;
                target_is_first &= (local_player_count == 0);
            }
            ++local_player_count;
        }
    }

    if ( matched_controller == -1 && !we_are_at_the_main_menu )
        local_player_index = -1;

    if ( (unsigned int)local_player_count > 4 )
        return;

    /* choose the error widget for this player count / modality */
    if ( local_player_count <= 1 )
    {
        widget_name = modal ? "ui\\shell\\error\\error_modal_fullscreen"
                            : "ui\\shell\\error\\error_nonmodal_fullscreen";
    }
    else if ( local_player_count == 2 )
    {
        widget_name = modal ? "ui\\shell\\error\\error_modal_halfscreen"
                            : "ui\\shell\\error\\error_nonmodal_halfscreen";
    }
    else if ( local_player_count == 3 )
    {
        if ( (unsigned char)target_is_first == 1 )
            widget_name = modal ? "ui\\shell\\error\\error_modal_halfscreen"
                                : "ui\\shell\\error\\error_nonmodal_halfscreen";
        else
            widget_name = modal ? "ui\\shell\\error\\error_modal_qtrscreen"
                                : "ui\\shell\\error\\error_nonmodal_qtrscreen";
    }
    else /* count == 4 */
    {
        widget_name = modal ? "ui\\shell\\error\\error_modal_qtrscreen"
                            : "ui\\shell\\error\\error_nonmodal_qtrscreen";
    }

    /* if we're fading the main menu away, kick the music + map change once */
    if ( we_are_at_the_main_menu
      && widget_globals.fade_to_black <= 1.0f
      && widget_globals.fade_to_black >= 0.0f )
    {
        if ( !widget_globals.main_menu_music_active )
            ui_start_main_menu_music();
        main_defer_map_map_change();
        widget_globals.fade_to_black = -1.0f;
    }

    {
        int slot = (local_player_index == -1) ? 0 : local_player_index;
        widget_instance *current = widget_globals.active_widgets[slot];
        int invoking_tag;
        widget_instance *dialog;

        if ( current )
        {
            invoking_tag = current->ui_widget_tag_index;
            if ( current->widget_is_error_dialog == 1 )
                return;   /* an error dialog is already up */
        }
        else
        {
            invoking_tag = -1;
        }

        dialog = ui_widget_load_by_name_or_tag(widget_name, -1, 0, local_player_index, invoking_tag, -1, -1);
        if ( !dialog )
            return;

        {
            __int16 string_index;
            if ( error_code >= 0 )
                string_index = (error_code <= NUMBER_OF_ERROR_CODES - 1) ? error_code : NUMBER_OF_ERROR_CODES - 1;
            else
                string_index = 0;
            dialog->children->children->parameters.text_box_parameters.string_list_index = string_index;
        }

        if ( !dialog->pause_game_time )
        {
            dialog->pause_game_time = pause_game_time;
            if ( pause_game_time == 1 && game_connection() != _game_connection_network_server )
            {
                ++widget_globals.pause_game_time_count;
                if ( !game_time_get_paused() )
                    game_time_set_paused(1u);
                if ( !widget_globals.sound_paused && !we_are_at_the_main_menu )
                {
                    sound_pause(1u);
                    widget_globals.sound_paused = 1;
                }
            }
        }

        if ( error_code == _error_controller_unplugged_start_to_continue || error_code == _error_controller_unplugged )
        {
            dialog->close_if_local_player_controller_present = (error_code == _error_controller_unplugged) ? 1 : 0;
            dialog->auto_close_fade_time = 0;
            dialog->milliseconds_to_auto_close = 0;
        }
        else
        {
            dialog->close_if_local_player_controller_present = 0;
        }
    }
}
