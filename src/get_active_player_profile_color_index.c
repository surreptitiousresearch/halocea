/* get_active_player_profile_color_index @0x8377F880 — sets a widget's animation frame index to the active
 * local player's chosen primary color index, clamped to the number of colors currently available (or 0 if
 * the profile has no color chosen). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern uint16_t player_profile_number_of_available_primary_colors(void);

void get_active_player_profile_color_index(widget_instance *widget)
{
    player_profile profile;
    player_ui_get_active_player_profile(widget->local_player_index, &profile);

    if ( profile.primary_color_index >= 0 )
    {
        int last_available_color = player_profile_number_of_available_primary_colors() - 1;
        widget->animation_data.current_frame_index =
            profile.primary_color_index <= last_available_color ? profile.primary_color_index : last_available_color;
    }
    else
    {
        widget->animation_data.current_frame_index = 0;
    }
}
