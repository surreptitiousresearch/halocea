/* get_editable_playlist_profile_display_name @0x8377F7F0 — copy the profile-being-edited game variant's
 * human-readable description into the widget's text-box buffer (reallocated to fit, 23 chars + terminator). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/game_variant.h"

extern game_variant * player_ui_get_edit_playlist_profile(void);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

void get_editable_playlist_profile_display_name(widget_instance *widget)
{
    game_variant *edit_playlist_profile = player_ui_get_edit_playlist_profile();

    if ( edit_playlist_profile )
    {
        uint16_t *text = ui_widget_realloc(widget->parameters.text_box_parameters.text, 0x30u);

        widget->parameters.text_box_parameters.text = text;
        if ( text )
        {
            ustrncpy(text, edit_playlist_profile->human_readable_game_description, 0x17u);
            widget->parameters.text_box_parameters.text[23] = 0;
        }
    }
}
