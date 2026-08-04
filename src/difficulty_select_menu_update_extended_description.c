/* difficulty_select_menu_update_extended_description @0x8377E58C — refresh the difficulty-select list's
 * extended-description panel: if a difficulty is selected, stamps its index onto the description's icon
 * frame and its title text's string_list_index; always stamps the active player's name onto the next text
 * widget, then highlights (frame index 1/0) whichever of the 4 difficulty child widgets matches the
 * selection. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);

void difficulty_select_menu_update_extended_description(widget_instance *list_widget)
{
    unsigned __int16 selected_list_item_index = list_widget->parameters.list_parameters.selected_list_item_index;
    widget_instance *icon = list_widget->parameters.list_parameters.extended_description->children;
    widget_instance *title = icon->next;

    if ( selected_list_item_index != 0xFFFF )
    {
        icon->animation_data.current_frame_index = selected_list_item_index;
        title->parameters.text_box_parameters.string_list_index = selected_list_item_index;
    }

    player_profile profile;
    player_ui_get_active_player_profile(0, &profile);
    set_profile_name(title->next, profile.player_name);

    widget_instance *child = list_widget->children;
    for ( __int16 i = 0; i < 4; i++ )
    {
        child->animation_data.current_frame_index = (i == list_widget->parameters.list_parameters.selected_list_item_index);
        child = child->next;
    }
}
