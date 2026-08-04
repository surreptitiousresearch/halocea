/* difficulty_menu_initialize @0x83697770 — widget callback: preselect the difficulty menu. If there is a
 * valid persistent campaign marker for the current map, preselect its saved difficulty and focus the
 * matching child; otherwise default to difficulty 1 (the second child). Sets both the text-box string list
 * index and the list selection to the chosen difficulty. */
#include <stdint.h>

#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/persistant_game_data_info.h"

extern char *main_get_map_name(void);
extern int stricmp(const char *string1, const char *string2);
extern widget_instance *widget_instance_get_nth_child(widget_instance *widget, int index);

uint8_t difficulty_menu_initialize(widget_instance *widget, event_record *event, unsigned __int8 *widget_deleted)
{
    widget_instance *focused_child;
    __int16 difficulty;

    if ( persistant_game_data_info.valid != 1
        || stricmp(persistant_game_data_info.map_name, main_get_map_name()) )
    {
        focused_child = widget_instance_get_nth_child(widget, 1);
        difficulty = 1;
    }
    else
    {
        focused_child = widget_instance_get_nth_child(widget, persistant_game_data_info.difficulty);
        difficulty = persistant_game_data_info.difficulty;
    }
    widget->parameters.text_box_parameters.string_list_index = difficulty;
    widget->focused_child = focused_child;
    widget->parameters.list_parameters.selected_list_item_index = difficulty;
    return 1;
}
