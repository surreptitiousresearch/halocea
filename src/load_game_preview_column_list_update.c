/* load_game_preview_column_list_update @0x83698388 — per-frame refresh of the "load game" saved-game
 * picker's single-preview-column list widget: rebuilds the list, updates the description panel's player
 * name, then either fills in the selected save's summary (string_list_index/frame index from the record's
 * first dword, a difficulty star icon lit per the record's second dword being 1/2/3, and the save's
 * description string copied from byte offset 8 of the opaque preview-list record) or hides everything when
 * nothing is selected. Sibling of player_profile_single_preview_column_list_update.c's identical structural
 * shape, but with no dedicated header for this list's opaque per-item record. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/player_profile.h"
#include "headers/game_difficulty.h"

extern uint8_t single_preview_column_list_get_string(uint16_t *out, int index, void *list_items);
extern void single_preview_column_list_update(widget_instance *widget,
        unsigned __int8 (*get_string)(unsigned __int16 *, int, void *));
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void set_profile_name(widget_instance *text_widget, const wchar_t *name);
extern void * preview_list_get_data_for_item(int index);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

void load_game_preview_column_list_update(widget_instance *widget)
{
    single_preview_column_list_update(widget, single_preview_column_list_get_string);

    player_profile active_profile;
    player_ui_get_active_player_profile(0, &active_profile);
    set_profile_name(widget->parameters.list_parameters.extended_description->children, active_profile.player_name);

    widget_instance *frame_widget = widget->parameters.list_parameters.extended_description->children->next;
    widget_instance *string_list_widget = frame_widget->children;
    widget_instance *description_widget = string_list_widget->next;
    widget_instance *star1 = description_widget->next;
    widget_instance *star2 = star1->next;
    widget_instance *star3 = star2->next;

    const int *data_for_item = preview_list_get_data_for_item(
        widget->parameters.list_parameters.selected_list_item_index);

    frame_widget->animation_data.current_frame_index = 0;

    if ( data_for_item )
    {
        int difficulty = data_for_item[1];

        star1->animation_data.current_frame_index = 1;
        star1->visible = (difficulty == game_difficulty_level_normal);
        star2->animation_data.current_frame_index = 2;
        star2->visible = (difficulty == game_difficulty_level_hard);
        star3->animation_data.current_frame_index = 3;
        star3->visible = (difficulty == game_difficulty_level_impossible);

        string_list_widget->parameters.text_box_parameters.string_list_index = *data_for_item;
        frame_widget->animation_data.current_frame_index = *data_for_item;

        unsigned short *description_text = ui_widget_realloc(
            description_widget->parameters.text_box_parameters.text, 0x40u);
        description_widget->parameters.text_box_parameters.text = description_text;
        /* RAW: opaque preview-list record has no DB struct — +8 is the wide description text */
        ustrncpy(description_text, (const wchar_t *)((const char *)data_for_item + 8), 0x1F);
        description_widget->parameters.text_box_parameters.text[31] = 0;
    }
    else
    {
        star1->visible = 0;
        star2->visible = 0;
        star3->visible = 0;
        string_list_widget->parameters.text_box_parameters.string_list_index = 0;
        frame_widget->animation_data.current_frame_index = 0;
    }
}
