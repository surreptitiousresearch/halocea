/* new_campaign_if_no_custom_player_profiles_exist @0x83697848 — widget callback: if the player already has
 * at least one custom (non-default) profile, allow the menu to proceed (return 1). Otherwise begin the
 * new-campaign name-entry flow: seed the entered-name buffer with an auto-generated "untitled" name, clear
 * the begin-editing flag, remember the event's controller, launch the virtual keyboard, and return 0.
 *
 * new_campaign_entered_name/new_campaign_name_cursor are un-symbolized adjacent globals (see new_campaign_chosen.c). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"

extern uint16_t new_campaign_entered_name[11];
extern int16_t new_campaign_name_cursor;

extern void player_profiles_enumerate_available_to_local_player_index(int16_t local_player_index, uint16_t *number_of_profiles, int *player_profile_indices, uint8_t include_default_profiles);
extern void saved_game_file_get_useable_untitled_profile_name(uint16_t *display_name);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern uint8_t virtual_keyboard_launch(uint16_t *text_buffer, uint16_t buffer_size, int16_t caption_index);

uint8_t new_campaign_if_no_custom_player_profiles_exist(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    int16_t number_of_profiles = 1;
    int profile_indices[3];
    wchar_t generated_name[152];

    player_profiles_enumerate_available_to_local_player_index(-1, (uint16_t *)&number_of_profiles,
            profile_indices, 0);
    if ( number_of_profiles > 0 )
        return 1;

    saved_game_file_get_useable_untitled_profile_name(generated_name);
    ustrncpy(new_campaign_entered_name, generated_name, 0xBu);
    new_campaign_name_cursor = 0;
    new_campaign_begin_editing = 0;
    new_campaign_controller_index = event->controller_index;
    virtual_keyboard_launch(new_campaign_entered_name, 0x18u, 8);
    return 0;
}
