/* get_editable_player_profile_display_name @0x8377F780 — copy the profile-being-edited player's name into
 * the widget's text-box buffer (reallocated to fit, 11 chars + terminator). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern player_profile * player_ui_get_edit_player_profile(void);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

void get_editable_player_profile_display_name(widget_instance *widget)
{
    player_profile *edit_player_profile = player_ui_get_edit_player_profile();

    if ( edit_player_profile )
    {
        unsigned __int16 *text = ui_widget_realloc(widget->parameters.text_box_parameters.text, 0x18u);

        widget->parameters.text_box_parameters.text = text;
        if ( text )
        {
            ustrncpy(text, edit_player_profile->player_name, 0xBu);
            widget->parameters.text_box_parameters.text[11] = 0;
        }
    }
}
