/* network_game_cancel @0x83692270 — menu back/cancel handler: tear down the global network game
 * server and client and clear the multiplayer variant UI state. (Decompiler call-argument
 * threading is register-leftover noise; the dispose_* callees are void(void).) */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern void dispose_global_network_game_server(void);
extern void dispose_global_network_game_client(void);
extern void player_ui_clear_multiplayer_variant(void);

uint8_t network_game_cancel(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    dispose_global_network_game_server();
    dispose_global_network_game_client();
    player_ui_clear_multiplayer_variant();
    return 1;
}
