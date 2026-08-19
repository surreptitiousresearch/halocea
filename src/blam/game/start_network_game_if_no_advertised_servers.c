/* start_network_game_if_no_advertised_servers @0x83697470 — widget callback: if the server list widget has
 * no items and a network game client exists, start a brand-new advertised server; otherwise do nothing. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/network_game_client.h"

extern network_game_client *global_network_game_client_get(void);
extern uint8_t network_game_start_new_server(widget_instance *widget, event_record *event, uint8_t *widget_deleted);

uint8_t start_network_game_if_no_advertised_servers(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    if ( widget->parameters.list_parameters.number_of_items || !global_network_game_client_get() )
        return 0;
    return network_game_start_new_server(widget, event, widget_deleted);
}
