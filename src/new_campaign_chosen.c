/* new_campaign_chosen @0x83697598 — widget callback for starting a new campaign: seed the entered-name
 * buffer with an auto-generated "untitled" profile name, clear the begin-editing flag, remember the event's
 * controller, and launch the virtual keyboard so the player can name the campaign profile.
 *
 * new_campaign_entered_name/new_campaign_name_cursor are un-symbolized adjacent globals (an 11-wchar_t entered-name buffer
 * followed by a separate global used as its extra terminator; the DB names neither, so the raw addresses
 * are used verbatim per project convention — matches new_campaign_decision.c). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"

extern unsigned __int16 new_campaign_entered_name[11];
extern __int16 new_campaign_name_cursor;

extern void saved_game_file_get_useable_untitled_profile_name(uint16_t *display_name);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern uint8_t virtual_keyboard_launch(uint16_t *text_buffer, uint16_t buffer_size, int16_t caption_index);

uint8_t new_campaign_chosen(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    wchar_t generated_name[144];

    saved_game_file_get_useable_untitled_profile_name(generated_name);
    ustrncpy(new_campaign_entered_name, generated_name, 0xBu);
    new_campaign_name_cursor = 0;
    new_campaign_begin_editing = 0;
    new_campaign_controller_index = event->controller_index;
    virtual_keyboard_launch(new_campaign_entered_name, 0x18u, 8);
    return 1;
}
