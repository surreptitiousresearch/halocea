/* network_game_remove_local_player @0x83696C68 — widget callback: quit the network game for the local
 * player owning the event's controller. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void network_game_client_local_player_quit(int16_t local_player_index);

uint8_t network_game_remove_local_player(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    network_game_client_local_player_quit(event->controller_index);
    return 1;
}
