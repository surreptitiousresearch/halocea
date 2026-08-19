/* solo_level_select_list_update_item @0x8377E460 — refresh a campaign level-select list row's 6 child
 * widgets (level name text, thumbnail frame, difficulty-completion text, and 3 difficulty checkmark
 * icons) for level `index`: if any difficulty has been completed, shows the level's own name/thumbnail
 * and per-difficulty checkmark visibility (highlighting the most-recently-played level specially);
 * otherwise shows a generic "locked" placeholder (frame/string 10) with all checkmarks hidden. */

#include "headers/widget_instance.h"
#include "headers/single_player_level_data.h"
#include "headers/persistant_game_data_info.h"

void solo_level_select_list_update_item(widget_instance *item, int index)
{
    widget_instance *name_text = item->children;
    widget_instance *thumbnail = name_text->next;
    widget_instance *status_text = thumbnail->next;
    widget_instance *check1 = status_text->next;
    widget_instance *check2 = check1->next;
    widget_instance *check3 = check2->next;

    check1->animation_data.current_frame_index = 1;
    check2->animation_data.current_frame_index = 2;
    check3->animation_data.current_frame_index = 3;

    if ( single_player_level_data[index].level_completed[0]
      || single_player_level_data[index].level_completed[1]
      || single_player_level_data[index].level_completed[2]
      || single_player_level_data[index].level_completed[3] )
    {
        name_text->parameters.text_box_parameters.string_list_index = index;
        thumbnail->animation_data.current_frame_index = index;
        status_text->parameters.text_box_parameters.string_list_index = index;

        if ( persistant_game_data_info.valid == 1 && index == persistant_game_data_info.map_index )
            status_text->parameters.text_box_parameters.string_list_index = 11;

        check1->visible = single_player_level_data[index].level_completed[1];
        check2->visible = single_player_level_data[index].level_completed[2];
        check3->visible = single_player_level_data[index].level_completed[3];
    }
    else
    {
        name_text->parameters.text_box_parameters.string_list_index = 10;
        thumbnail->animation_data.current_frame_index = 10;
        status_text->parameters.text_box_parameters.string_list_index = 10;
        check1->visible = 0;
        check2->visible = 0;
        check3->visible = 0;
    }
}
