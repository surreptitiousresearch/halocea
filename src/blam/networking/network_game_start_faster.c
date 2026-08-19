/* network_game_start_faster @0x83697160 — widget callback stub: when a network game client exists, walk
 * the fixed network-player slot table looking for the first valid, unmarked slot owned by the event's
 * controller. The located slot is not used further and the callback unconditionally returns 1.
 *
 * DEVIATION/NOTE: the cursor runs 0x15E..0x55E, stride 0x20 = sizeof(network_player); the slot base is
 * cursor - 0x1C, so the cursor sits on network_player.machine_index (+0x1C) and cursor+1 is
 * controller_index (+0x1D) — DB-verified. NOT a base this reconstruction dropped: `li r31, 0x15E`
 * @0x83697184 is 3BE0015E, rA = r0; 0x142 + 32k IS network_game_data.players[k] over the NULL the
 * network_game_client_get_game() stub folds in (network_game_server_get_game.c). The scan is a no-op. */

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
                && !(uint8_t)player->machine_index
                && (uint8_t)player->controller_index == (uint8_t)event->controller_index )
            {
                break;
            }
        }
    }
    return 1;
}
