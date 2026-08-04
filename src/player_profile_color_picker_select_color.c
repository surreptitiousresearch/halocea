/* player_profile_color_picker_select_color @0x836962D8 — widget callback: map the selected color-swatch
 * list item to its underlying color list index and store it as the edited profile's primary color. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"

extern player_profile * player_ui_get_edit_player_profile(void);
extern int preview_list_get_list_index_for_item(int index);

uint8_t player_profile_color_picker_select_color(widget_instance *screen_widget, event_record *event, uint8_t *widget_deleted)
{
    player_profile *edit_player_profile = player_ui_get_edit_player_profile();
    __int16 selected_list_item_index = screen_widget->parameters.list_parameters.selected_list_item_index;
    int list_index_for_item = preview_list_get_list_index_for_item(selected_list_item_index);

    if ( !edit_player_profile )
        return 0;
    edit_player_profile->primary_color_index = list_index_for_item;
    return 1;
}
