/* single_player_set_player2_controller_choice @0x836973B8 — widget callback: assign the event's controller
 * as single-player local player 1's controller. If it is the same controller already driving player 0,
 * report the co-op controller conflict, mark the widget deleted, and reject the choice. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/error_type.h"

extern int16_t player_ui_get_single_player_local_player_controller(int16_t local_player_index);
extern void player_ui_set_single_player_local_player_controller(int16_t local_player_index, int16_t controller_index);
extern void display_error(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);

uint8_t single_player_set_player2_controller_choice(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    int16_t controller_index = event->controller_index;
    if ( controller_index == player_ui_get_single_player_local_player_controller(0) )
    {
        display_error(_error_controller_coop_controller_conflict, -1, 1u, 0);
        *widget_deleted = 1;
        return 0;
    }
    player_ui_set_single_player_local_player_controller(1, controller_index);
    return 1;
}
