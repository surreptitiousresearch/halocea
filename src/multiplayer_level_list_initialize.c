/* multiplayer_level_list_initialize @0x83692420 — populate the multiplayer map-select list widget:
 * point it at the runtime multiplayer map array, and select the last-used map by name if one was saved
 * (falling back to the first entry), then (re)build the preview list bank with each map's friendly name,
 * marking the selected entry as default. Always returns 1. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/multiplayer_map_s.h"
#include "headers/blam_data_globals.h"


extern int map_list_get_count(void);
extern uint8_t saved_game_file_retrieve_last_used_multiplayer_map(char *map_name);
extern int stricmp(const char *a, const char *b);
extern void preview_list_create(void);
extern char * map_list_get_level_name(int index);
extern void map_list_get_friendly_level_name(const char *map_name, uint16_t *name_buf, int name_buf_len);
extern void preview_list_add_item_to_bank(int bank, const wchar_t *name, int list_index, void *data,
    unsigned int data_len, char is_default);

uint8_t multiplayer_level_list_initialize(widget_instance *list_widget, event_record *event,
    uint8_t *widget_deleted)
{
    int count = map_list_get_count();
    list_widget->parameters.list_parameters.number_of_items = count;
    list_widget->parameters.list_parameters.list_items = multiplayer_maps;

    char last_used_map_name[256];
    if ( saved_game_file_retrieve_last_used_multiplayer_map(last_used_map_name) )
    {
        list_widget->parameters.text_box_parameters.string_list_index = 0;

        if ( count > 0 )
        {
            while ( stricmp(last_used_map_name, multiplayer_maps[list_widget->parameters.text_box_parameters.string_list_index].name) )
            {
                int16_t next_index = (int16_t)(list_widget->parameters.text_box_parameters.string_list_index + 1);
                list_widget->parameters.text_box_parameters.string_list_index = next_index;
                if ( next_index >= count )
                    break;
            }
        }

        if ( list_widget->parameters.text_box_parameters.string_list_index == count )
            list_widget->parameters.text_box_parameters.string_list_index = 0;
    }

    int16_t selected_index = list_widget->parameters.text_box_parameters.string_list_index;
    list_widget->parameters.list_parameters.list_item_top_index = -1;
    list_widget->parameters.list_parameters.selected_list_item_index = selected_index;

    preview_list_create();

    for ( int map_index = 0; map_index < count; map_index++ )
    {
        const char *level_name = map_list_get_level_name(map_index);
        wchar_t friendly_name[280];
        map_list_get_friendly_level_name((char *)level_name, friendly_name, 256);
        preview_list_add_item_to_bank(0, friendly_name, map_index, nullptr, 0,
            list_widget->parameters.text_box_parameters.string_list_index == map_index);
    }

    return 1;
}
