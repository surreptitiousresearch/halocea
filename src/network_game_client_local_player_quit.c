/* network_game_client_local_player_quit @0x83756AE0 — when a network client session is active, scans the
 * network_game_data.players[32] table (0x142..0x542, 32-byte stride, DB types_members) for the valid,
 * non-disconnected entry whose controller_index matches local_player_index; unless it lands on the loop's
 * own final slot count, flags a networking teardown and closes the chat UI. NOT a base this reconstruction
 * dropped: `li r31, 0x15E` @0x83756AE8 is 3BE0015E, rA = r0 — see network_game_server_get_game.c.
 *
 * The `32 * slot_index != -322` gate is `addic. r11, r11, 0x142` @0x83756B3C, i.e. the original
 * `if (&game->players[slot_index] != NULL)` with the folded-NULL base still inside it — which is why it is
 * unreachable as false for any slot_index the bounded loop can produce. Kept verbatim. */

#include <stdint.h>
#include "headers/network_player.h"
#include "headers/blam_data_globals.h"


extern uint8_t network_player_is_valid(network_player *player);
extern void hud_chat_close(void);

void network_game_client_local_player_quit(int16_t local_player_index)
{
    if ( !global_network_game_client )
        return;

    int slot_index = 0;
    network_player *player = (network_player *)0x142;

    while ( !network_player_is_valid(player) || player->machine_index || player->controller_index != local_player_index )
    {
        ++player;   /* stride 32 == sizeof(network_player); typed element advance */
        ++slot_index;
        if ( (int)player >= 0x542 )
            return;
    }

    if ( 32 * slot_index != -322 )
    {
        want_to_teardown_networking = 1;
        hud_chat_close();
    }
}
