/* network_game_start_slower @0x836971E8 — widget callback stub, identical in behavior to
 * network_game_start_faster: when a network game client exists, scan the fixed network-player slot table
 * for the first valid, unmarked slot owned by the event's controller. The result is unused; returns 1.
 *
 * DEVIATION/NOTE: raw slot-address iteration (0x15E..0x55E, stride 0x20 = sizeof(network_player))
 * reproduced verbatim from the disassembly; the slot base passed to network_player_is_valid is
 * (cursor - 0x1C), so the cursor points at network_player.machine_index (+0x1C) and cursor+1 is
 * controller_index (+0x1D) — DB-verified (an earlier comment called these a marked flag). The scan
 * has no effect on the return value. */

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
