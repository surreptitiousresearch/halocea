/* clear_multiplayer_player_joins @0x83692070 — menu handler: tear down any global network game
 * client/server and reset the local multiplayer join/variant UI state. (The decompiler's chained
 * argument threading is register-leftover noise: every callee here is void(void).) */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void dispose_global_network_game_client(void);
extern void dispose_global_network_game_server(void);
extern void player_ui_clear_multiplayer_joins(void);
extern void player_ui_clear_multiplayer_variant(void);

uint8_t clear_multiplayer_player_joins(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    dispose_global_network_game_client();
    dispose_global_network_game_server();
    player_ui_clear_multiplayer_joins();
    player_ui_clear_multiplayer_variant();
    return 1;
}
