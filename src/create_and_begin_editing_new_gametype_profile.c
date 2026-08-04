/* create_and_begin_editing_new_gametype_profile @0x83696F28 — widget callback: create a new playlist
 * (gametype) profile with an auto-generated name, begin editing it, seed its contents from either the
 * currently-selected preview-list item or a fresh Slayer default, clear its game-mode flag bits, and
 * launch the on-screen keyboard to let the player rename it. On successful launch, remembers the
 * directory the profile was saved to for next time. Returns whether the keyboard was actually launched;
 * on failure, shows an error and plays the failure sound. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/game_engine_universal_variant_flags.h"
#include "headers/error_type.h"
#include "headers/virtual_keyboard_validate_mode.h"

extern void saved_game_file_get_useable_untitled_profile_name(uint16_t *display_name);
extern int playlist_profile_new(int16_t local_player_index, uint16_t *name);
extern void player_ui_begin_editing_profile(int profile_index);
extern game_variant * player_ui_get_edit_playlist_profile(void);
extern void player_ui_end_editing_profile(void);
extern int preview_list_get_list_index_for_item(int index);
extern void * preview_list_get_data_for_item(int index);
extern game_variant * build_game_variant_slayer(game_variant *result);
extern void *memcpy(void *destination, const void *source, unsigned int size);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern uint8_t virtual_keyboard_launch(uint16_t *text_buffer, uint16_t buffer_size, int16_t caption_index);
extern void virtual_keyboard_set_validation(int validate_mode);
extern uint8_t saved_game_file_get_path_to_enclosing_directory(int index, char *full_path);
extern void saved_game_file_remember_last_used_multiplayer_variant_directory(char *directory_path);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t create_and_begin_editing_new_gametype_profile(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    unsigned __int8 launched = 0;
    widget_instance *parent = widget->parent->parent;

    unsigned __int16 profile_name[128];
    saved_game_file_get_useable_untitled_profile_name(profile_name);

    if ( !profile_name[0] )
        goto fail;

    int profile_index = playlist_profile_new(widget->local_player_index, profile_name);
    if ( profile_index == -1 )
        goto fail;

    player_ui_begin_editing_profile(profile_index);
    game_variant *edit_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_profile )
    {
        player_ui_end_editing_profile();
        goto fail;
    }

    int list_index = preview_list_get_list_index_for_item(parent->parameters.list_parameters.selected_list_item_index);
    const void *source_data;
    game_variant slayer_default;

    if ( list_index == -1 )
    {
        build_game_variant_slayer(&slayer_default);
        source_data = &slayer_default;
    }
    else
    {
        source_data = preview_list_get_data_for_item(list_index);
    }

    memcpy(edit_profile, source_data, sizeof(game_variant));
    edit_profile->flags = 0;
    ustrncpy(edit_profile->human_readable_game_description, (const wchar_t *)profile_name, 0x17u);
    edit_profile->human_readable_game_description[23] = 0;
    /* clear the two game-mode bits (7,8); 0xFFFFFE7F == ~((1<<7)|(1<<8)) */
    edit_profile->universal_variant.flags &= ~((1u << _game_mode_classic_bit) | (1u << _game_mode_standard_bit));

    launched = virtual_keyboard_launch(edit_profile->human_readable_game_description, 0x30u, 9);
    if ( launched == 1 )
    {
        virtual_keyboard_set_validation(_virtual_keyboard_validate_playlist_profile_name);

        char enclosing_directory[304]; /* matches the shipped code's reuse of a 2-element game_variant stack buffer */
        if ( saved_game_file_get_path_to_enclosing_directory(profile_index, enclosing_directory) )
            saved_game_file_remember_last_used_multiplayer_variant_directory(enclosing_directory);
    }

fail:
    if ( !launched )
    {
        display_error_deferred(_error_unable_to_create_multiplayer_game_file, -1, 1u, 0);
        ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
    }

    return launched;
}
