/* netgame_unjoin_player @0x836974C8 */
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/network_game_client.h"

extern network_game_client *global_network_game_client_get(void);

uint8_t netgame_unjoin_player(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    global_network_game_client_get();
    return 1;
}
