/* multiplayer_profile_set_for_game @0x83692868 — widget callback: apply the selected multiplayer game
 * variant profile to the pending game. The selected list item's raw index (v7) encodes: -1 = nothing
 * selected (fail silently with a feedback sound), >= 0 = an invalid/unsupported selection (fail with an
 * error dialog), otherwise a negative-encoded playlist profile reference passed to playlist_profile_get.
 * On success, remembers the profile's enclosing directory as the last-used location and applies the
 * variant. If a server game is already in progress (game_connection() == _game_connection_network_server), forces the playlist to
 * reinitialize and ends the current game to pick up the new variant.
 *
 * DEVIATION: the decompiler chains game_engine_playlist_initialize's return into ui_widgets_close_all and
 * that into game_engine_end_game as if each took the previous call's result; per their DB prototypes (0/0/1
 * args respectively) these are three independent calls that merely reuse r3 without clearing it — the same
 * "threaded-r3 artifact" documented in player_ui_fast_setup_network_server.c / game_engine_end_game.c. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/game_connection.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/error_type.h"

#include "headers/network_game_server.h"
extern int preview_list_get_list_index_for_item(int index);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern uint8_t playlist_profile_get(int playlist_profile_index, game_variant *variant);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t saved_game_file_get_path_to_enclosing_directory(int index, char *full_path);
extern void saved_game_file_remember_last_used_multiplayer_variant_directory(char *directory_path);
extern void player_ui_set_game_variant(game_variant *variant);
extern int16_t game_connection(void);
extern void game_engine_playlist_initialize(unsigned __int8 force_begin_playlist);
extern void ui_widgets_close_all(void);
extern void game_engine_end_game(void);

uint8_t multiplayer_profile_set_for_game(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted)
{
    int success = 1;

    int list_index = preview_list_get_list_index_for_item(wrapper_widget->parameters.list_parameters.selected_list_item_index);
    int *list_items = (int *)wrapper_widget->parameters.list_parameters.list_items;
    int item_value = list_items[list_index];

    if ( item_value == -1 || item_value >= 0 )
    {
        if ( item_value >= 0 )
            display_error_deferred(_error_warning_saved_game_file_damaged, -1, 1u, 0);
        ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
        success = 0;
    }
    else
    {
        game_variant variant;
        if ( !playlist_profile_get(list_items[list_index], &variant) )
        {
            success = 0;   /* no feedback sound here, unlike the item_value>=0/-1 failure paths above */
        }
        else
        {
            global_network_game_server_get();

            char path[264];
            if ( saved_game_file_get_path_to_enclosing_directory(item_value, path) )
                saved_game_file_remember_last_used_multiplayer_variant_directory(path);

            player_ui_set_game_variant(&variant);
        }
    }

    if ( !success || game_connection() != _game_connection_network_server )
        return success;

    game_engine_playlist_initialize(0);
    ui_widgets_close_all();
    game_engine_end_game();
    return 0;
}
