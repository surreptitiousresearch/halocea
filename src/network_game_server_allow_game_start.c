/* network_game_server_allow_game_start @0x836972C0 */
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

typedef struct network_game_server network_game_server;
extern network_game_server *global_network_game_server_get(void);

uint8_t network_game_server_allow_game_start(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    global_network_game_server_get();
    return 1;
}
