/* display_error_if_no_network_connection @0x83697438 — widget callback: show the "no network connection"
 * error for the event's controller (modal, pausing game time) and reject the action. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/error_type.h"

extern void display_error(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);

uint8_t display_error_if_no_network_connection(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    display_error(_error_network_connected_play_no_network, event->controller_index, 1u, 1u);
    return 0;
}
