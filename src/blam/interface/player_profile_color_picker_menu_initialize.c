/* player_profile_color_picker_menu_initialize @0x83696180 — populate the primary-color picker list
 * widget: clamps the edit profile's stored primary_color_index into range and mirrors it into the
 * widget's text-box/list selection fields, then (re)allocates the list's item array and fills it from
 * the "colors_list" unicode_string_list tag, one preview-list bank-0 entry per available color, marking
 * the entry matching the current selection as default. Always returns 1. */

#include <wchar.h>
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"

extern uint16_t player_profile_number_of_available_primary_colors(void);
extern player_profile * player_ui_get_edit_player_profile(void);
extern void preview_list_create(void);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void preview_list_add_item_to_bank(int bank, const wchar_t *name, int list_index, void *data, unsigned int data_len, char is_default);

uint8_t player_profile_color_picker_menu_initialize(widget_instance *widget, event_record *event,
    uint8_t *widget_deleted)
{
    uint16_t available_colors = player_profile_number_of_available_primary_colors();
    player_profile *edit_player_profile = player_ui_get_edit_player_profile();
    preview_list_create();

    if ( edit_player_profile )
    {
        int16_t stored_color_index = edit_player_profile->primary_color_index;
        int16_t clamped_color_index = available_colors - 1;
        if ( stored_color_index >= 0 )
        {
            if ( stored_color_index <= clamped_color_index )
                clamped_color_index = stored_color_index;
        }
        else
        {
            clamped_color_index = 0;
        }
        edit_player_profile->primary_color_index = clamped_color_index;
        widget->parameters.text_box_parameters.string_list_index = clamped_color_index;
        widget->parameters.list_parameters.selected_list_item_index = edit_player_profile->primary_color_index;
        widget->parameters.list_parameters.list_item_top_index = -1;
    }

    void *list_items = ui_widget_realloc(widget->parameters.list_parameters.list_items, available_colors);
    widget->parameters.list_parameters.list_items = list_items;
    if ( list_items )
    {
        int colors_list_tag = tag_loaded(0x75737472u /* 'ustr' */,
            "ui\\shell\\main_menu\\settings_select\\player_setup\\player_profile_edit\\color_edit\\colors_list");

        for ( uint16_t color_index = 0; color_index < available_colors; color_index++ )
        {
            ((uint8_t *)list_items)[color_index] = (uint8_t)color_index;
            int is_selected = widget->parameters.text_box_parameters.string_list_index - color_index;
            const wchar_t *color_name = (const wchar_t *)unicode_string_list_get_string(colors_list_tag, color_index);
            preview_list_add_item_to_bank(0, color_name, color_index, 0, 0, is_selected == 0);
        }

        widget->parameters.list_parameters.number_of_items = available_colors;
    }

    return 1;
}
