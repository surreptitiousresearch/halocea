/* new_campaign_decision @0x83697600 — widget callback that finishes the "new campaign" flow after the
 * virtual keyboard closes: if a profile name was entered, try to create a new player profile with it; if
 * that name is already taken, fall back to an auto-generated "untitled" profile name and retry once. On
 * success, load and activate the new profile (optionally opening the profile editor) and kick off the
 * campaign map; on failure, bail back to the main menu with an error. Plays an error sound if the
 * keyboard was cancelled outright (no saved text). Returns whether a campaign was actually started.
 *
 * new_campaign_entered_name/new_campaign_name_cursor are un-symbolized adjacent globals (an 11-wchar_t entered-name buffer
 * immediately followed by a separate global that happens to serve as its extra terminator byte here —
 * the DB gives neither a name, so the raw addresses are used verbatim per project convention). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/error_type.h"
#include "headers/blam_data_globals.h"

extern uint16_t new_campaign_entered_name[11];
extern int16_t new_campaign_name_cursor;

extern uint8_t virtual_keyboard_last_exit_saved_text(void);
extern void player_ui_set_single_player_local_player_controller(int16_t local_player_index, int16_t controller_index);
extern int player_profile_new(int16_t local_player_index, const uint16_t *name);
extern void saved_game_file_get_useable_untitled_profile_name(uint16_t *display_name);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern uint8_t player_profile_get(int player_profile_index, player_profile *profile);
extern void player_ui_set_active_player_profile(int16_t local_player_index, int profile_index, player_profile *profile);
extern void player_ui_begin_editing_profile(int profile_index);
extern void main_set_map_name(const char *name);
extern void main_defer_map_map_change(void);
extern void main_goto_main_menu(void);
extern void display_error_deferred(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);

uint8_t new_campaign_decision(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    int started = 0;

    if ( (uint16_t)new_campaign_controller_index == 0xFFFF )
        return started;

    if ( !virtual_keyboard_last_exit_saved_text() )
    {
        new_campaign_controller_index = -1;
        return started;
    }

    if ( new_campaign_entered_name[0] )
    {
        player_ui_set_single_player_local_player_controller(0, new_campaign_controller_index);
        int profile_index = player_profile_new(new_campaign_controller_index, new_campaign_entered_name);

        if ( profile_index == -1 )
        {
            uint16_t untitled_name[128];
            saved_game_file_get_useable_untitled_profile_name(untitled_name);
            ustrncpy(new_campaign_entered_name, (const wchar_t *)untitled_name, 0xBu);
            new_campaign_name_cursor = 0;
            profile_index = player_profile_new(new_campaign_controller_index, new_campaign_entered_name);
        }

        if ( profile_index != -1 )
        {
            player_profile profile;
            if ( player_profile_get(profile_index, &profile) )
            {
                player_ui_set_active_player_profile(0, profile_index, &profile);
                if ( new_campaign_begin_editing )
                    player_ui_begin_editing_profile(profile_index);
                started = 1;
            }
        }

        if ( (uint8_t)started == 1 )
        {
            main_set_map_name(scenario_paths_0[0]);
            main_defer_map_map_change();
            new_campaign_controller_index = -1;
            return started;
        }

        main_goto_main_menu();
        display_error_deferred(_error_unable_to_create_player_profile, -1, 1u, 0);
    }

    ui_play_audio_feedback_sound(_ui_audio_feedback_flag_failure);
    new_campaign_controller_index = -1;
    return started;
}
