/* network_game_start_slower @0x836971E8 — widget callback stub, identical in behavior to
 * network_game_start_faster: when a network game client exists, scan the fixed network-player slot table
 * for the first valid, unmarked slot owned by the event's controller. The result is unused; returns 1.
 *
 * DEVIATION/NOTE: cursor 0x15E..0x55E, stride 0x20 = sizeof(network_player); the slot base passed to
 * network_player_is_valid is (cursor - 0x1C), so the cursor sits on network_player.machine_index (+0x1C)
 * and cursor+1 is controller_index (+0x1D) — DB-verified. NOT a base this reconstruction dropped:
 * `li r31, 0x15E` @0x8369720C is 3BE0015E, rA = r0; 0x142 + 32k IS network_game_data.players[k] over the
 * NULL the network_game_client_get_game() stub folds in (network_game_server_get_game.c). Scan is a no-op. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/network_player.h"
#include "headers/network_game_client.h"

extern network_game_client *global_network_game_client_get(void);
extern uint8_t network_player_is_valid(network_player *player);

uint8_t network_game_start_slower(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    int slot_cursor;

    if ( global_network_game_client_get() )
    {
        for ( slot_cursor = 0x15E; slot_cursor < 0x55E; slot_cursor += 0x20 )
        {
            network_player *player = (network_player *)(slot_cursor - 0x1C);
            if ( network_player_is_valid(player)
                && !(uint8_t)player->machine_index
                && (uint8_t)player->controller_index == (uint8_t)event->controller_index )
            {
                break;
            }
        }
    }
    return 1;
}
