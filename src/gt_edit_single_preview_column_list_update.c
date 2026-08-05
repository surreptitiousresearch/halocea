/* gt_edit_single_preview_column_list_update @0x83781590 — refresh the gametype-edit preview list widget:
 * runs the generic single-preview-column update, looks up the selected row's game_variant
 * (preview_list_array_bank record slot 1, see color_picker_get_string.c for the shared array), hands it to
 * multiplayer_settings_select_list_update_item, then dims and event-disables the second child of the 12th
 * child widget when the variant's flags bit 0 is set (e.g. a locked/built-in variant preview). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/game_variant.h"
#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"

extern void single_preview_column_list_update(widget_instance *widget,
        uint8_t (__fastcall *func)(uint16_t *, int, void *));
extern uint8_t gametype_select_get_string(uint16_t *out, int index, void *list_items);
extern void multiplayer_settings_select_list_update_item(widget_instance *item, game_variant *variant);
extern widget_instance *widget_instance_get_nth_child(widget_instance *widget, int index);


void gt_edit_single_preview_column_list_update(widget_instance *widget)
{
    single_preview_column_list_update(widget,
        (uint8_t (__fastcall *)(uint16_t *, int, void *))gametype_select_get_string);

    int selected_list_item_index = widget->parameters.list_parameters.selected_list_item_index;
    game_variant *variant = 0;
    if ( selected_list_item_index >= 0
      && selected_list_item_index < preview_list_array_bank[preview_list_current_bank].count )
    {
        variant = ((game_variant **)preview_list_array_bank[preview_list_current_bank].elements)[4 * selected_list_item_index + 1];
    }

    multiplayer_settings_select_list_update_item(widget->parameters.list_parameters.extended_description->children, variant);

    widget_instance *target = widget_instance_get_nth_child(widget, 11)->children->next;
    if ( (variant->flags & 1) != 0 )
    {
        target->never_receive_events = 1;
        target->alpha_modifier = 0.333f;
    }
    else
    {
        target->never_receive_events = 0;
        target->alpha_modifier = 1.0f;
    }
}
