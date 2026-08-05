/* playlist_profile_initialize_name @0x83694848 — menu init handler: (re)allocate the text-box's
 * 256-byte edit buffer and seed it with the edited profile's human-readable description (127 wide
 * chars, NUL-terminated). Reports 0 when no profile is being edited. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_variant.h"

extern game_variant * player_ui_get_edit_playlist_profile(void);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

uint8_t playlist_profile_initialize_name(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();
    if ( !edit_playlist_profile )
        return 0;

    uint16_t *text = ui_widget_realloc(
        widget->parameters.text_box_parameters.text, 0x100);
    widget->parameters.text_box_parameters.text = text;
    if ( text )
    {
        ustrncpy(text, edit_playlist_profile->human_readable_game_description, 0x7F);
        widget->parameters.text_box_parameters.text[127] = 0;
    }
    return 1;
}
