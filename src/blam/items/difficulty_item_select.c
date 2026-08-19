/* difficulty_item_select @0x836982B8 — main-menu difficulty list item handler: locate this widget's
 * position among its parent's first four children; when it is already the selected item, commit the
 * difficulty (read back through the compiled 5th-child/children/parent/parent pointer walk, which lands on
 * the list's selected index — reproduced verbatim), play the accept sound, and outside coop drop to a
 * local connection, switch the main menu to single player and re-remember player 1's profile. The parent's
 * selection is then moved to this item. Always returns 1 (event handled). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/game_difficulty.h"
#include "headers/ui_audio_feedback_sound.h"
#include "headers/blam_data_globals.h"


extern void main_set_difficulty(int16_t difficulty);
extern void ui_play_audio_feedback_sound(int16_t audio_feedback);
extern void game_connection_set(int16_t new_connection);
extern void main_menu_switch_to_single_player(void);
extern void player_ui_remember_player1_profile(uint8_t write_to_disk);

uint8_t difficulty_item_select(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    widget_instance *parent = widget->parent;
    int16_t item_index = 0;

    for ( widget_instance *child = parent->children; widget != child; child = child->next )
    {
        if ( ++item_index >= 4 )
            return 1;
    }

    if ( parent->parameters.list_parameters.selected_list_item_index == item_index )
    {
        /* compiled pointer walk: 5th child -> its children -> parent -> parent == the list widget */
        int16_t difficulty = parent->children->next->next->next->next
                                 ->children->parent->parent->parameters.list_parameters.selected_list_item_index;
        if ( difficulty < number_of_game_difficulty_levels )
        {
            main_set_difficulty(difficulty);
            ui_play_audio_feedback_sound(_ui_audio_feedback_forward);
        }
        if ( !coop_mode_selected )
        {
            game_connection_set(0);
            main_menu_switch_to_single_player();
            player_ui_remember_player1_profile(0);
        }
    }

    parent->parameters.list_parameters.selected_list_item_index = item_index;
    return 1;
}
