/* single_player_set_player1_controller_choice @0x83697388 — widget callback: assign the event's controller
 * as the single-player local player 0's controller. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void player_ui_set_single_player_local_player_controller(int16_t local_player_index, int16_t controller_index);

uint8_t single_player_set_player1_controller_choice(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    player_ui_set_single_player_local_player_controller(0, event->controller_index);
    return 1;
}
