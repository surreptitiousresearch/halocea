/* gt_select_single_preview_column_list_update @0x837814D8 — refresh the gametype-select preview list
 * widget: runs the generic single-preview-column update, stamps the active player's name onto the extended
 * description panel, then looks up the selected row's game_variant (preview_list_array_bank record slot 1,
 * see color_picker_get_string.c for the same 16-byte-stride array) and hands it to
 * multiplayer_settings_select_list_update_item. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"
#include "headers/game_variant.h"
#include "headers/dynamic_array.h"
#include "headers/bank_item_s.h"
#include "headers/blam_data_globals.h"

extern void single_preview_column_list_update(widget_instance *widget, uint8_t (*func)(uint16_t *, int, void *));
extern uint8_t gametype_select_get_string(uint16_t *out, int index, void *list_items);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);
extern void multiplayer_settings_select_list_update_item(widget_instance *item, game_variant *variant);


void gt_select_single_preview_column_list_update(widget_instance *widget)
{
    single_preview_column_list_update(widget, (uint8_t (*)(uint16_t *, int, void *))gametype_select_get_string);

    player_profile profile;
    player_ui_get_active_player_profile(0, &profile);
    set_profile_name(widget->parameters.list_parameters.extended_description->children, profile.player_name);

    int selected_list_item_index = widget->parameters.list_parameters.selected_list_item_index;
    game_variant *variant = 0;
    if ( selected_list_item_index >= 0
      && selected_list_item_index < preview_list_array_bank[preview_list_current_bank].count )
    {
        /* DEVIATION: `((game_variant **)…)[4 * i + 1]` folded the 16-byte bank_item_s stride into a
         * 4-byte-pointee subscript; 0x83781564 `slwi r10, r9, 4` scales by 16 and 0x83781570 loads
         * offset 4, i.e. bank_item_s[i].ptr. */
        variant = ((bank_item_s *)preview_list_array_bank[preview_list_current_bank].elements)[selected_list_item_index].ptr;
    }

    multiplayer_settings_select_list_update_item(widget->parameters.list_parameters.extended_description->children->next, variant);
}
