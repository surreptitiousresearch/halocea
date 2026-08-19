/* pause_game_set_text_box_for_inverted @0x837801A0 */
#include <stdint.h>
#include "headers/widget_globals.h"

extern widget_instance *widget_instance_get_topmost_parent(widget_instance *widget);
extern int16_t player_ui_get_single_player_local_player_from_controller(int16_t controller_index);
extern uint8_t player_ui_look_pitch_is_inverted(int16_t local_player_index);

void pause_game_set_text_box_for_inverted(widget_instance *widget)
{
    widget_instance *topmost_parent = widget_instance_get_topmost_parent(widget);
    int single_player_local_player = player_ui_get_single_player_local_player_from_controller(topmost_parent->local_player_index);

    widget->parameters.text_box_parameters.string_list_index =
        player_ui_look_pitch_is_inverted(single_player_local_player == -1 ? 0 : single_player_local_player) != 0;
}
