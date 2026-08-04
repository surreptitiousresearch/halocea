/* network_game_start_faster @0x83697160 — widget callback stub: when a network game client exists, walk
 * the fixed network-player slot table looking for the first valid, unmarked slot owned by the event's
 * controller. The located slot is not used further and the callback unconditionally returns 1.
 *
 * DEVIATION/NOTE: the binary iterates raw slot addresses (0x15E..0x55E, stride 0x20 = sizeof(network_player));
 * network_player_is_valid is passed the slot base (cursor - 0x1C), so the cursor points at
 * network_player.machine_index (+0x1C) and cursor+1 is controller_index (+0x1D) — DB-verified (an earlier
 * comment called these a marked flag). Reproduced verbatim from the disassembly — the scan has no effect
 * on the result, so this reads as a leftover/no-op probe. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/network_player.h"
#include "headers/network_game_client.h"

extern network_game_client *global_network_game_client_get(void);
extern uint8_t network_player_is_valid(network_player *player);

uint8_t network_game_start_faster(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    int slot_cursor;

    if ( global_network_game_client_get() )
    {
        for ( slot_cursor = 0x15E; slot_cursor < 0x55E; slot_cursor += 0x20 )
        {
            network_player *player = (network_player *)(slot_cursor - 0x1C);
            if ( network_player_is_valid(player)
                && !(unsigned __int8)player->machine_index
                && (unsigned __int8)player->controller_index == (unsigned __int8)event->controller_index )
            {
                break;
            }
        }
    }
    return 1;
}
