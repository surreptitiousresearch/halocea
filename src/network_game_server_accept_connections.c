#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/network_game_server.h"

extern network_game_server *global_network_game_server_get(void);

uint8_t network_game_server_accept_connections(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    global_network_game_server_get();
    return 1;
}
