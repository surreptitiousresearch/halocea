#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/player_profile.h"

extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

void get_active_player_profile_display_name(widget_instance *widget)
{
    player_profile profile;

    player_ui_get_active_player_profile(widget->local_player_index, &profile);

    unsigned __int16 *text = ui_widget_realloc(widget->parameters.text_box_parameters.text, 0x18u);
    widget->parameters.text_box_parameters.text = text;
    if ( text )
    {
        ustrncpy(text, profile.player_name, 0xBu);
        widget->parameters.text_box_parameters.text[11] = 0;
    }
}
