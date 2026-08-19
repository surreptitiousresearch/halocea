/* network_game_player_is_local @0x837566F8 — is this network player local to this machine?
 * A valid player is local when its machine_index is 0 (this machine). For an invalid/placeholder
 * player it is treated as local unless we are in film playback (game_connection() ==
 * _game_connection_film_playback), where locality still hinges on machine_index 0. */

#include <stdint.h>
#include "headers/network_player.h"
#include "headers/game_connection.h"

extern uint8_t network_player_is_valid(network_player *player);
extern int16_t game_connection(void);

/* CAVEAT: in the film-playback branch the binary dereferences the player pointer that its
 * single entry null-check (cmplwi cr6,r3,0 / beq @0x837566F8) does not dominate — lbz
 * r11,0x1C(r31) runs with a null-reachable r31, exactly as this source reads. */
uint8_t network_game_player_is_local(network_player *player)
{
    if (player && network_player_is_valid(player))
        return player->machine_index == 0;
    if (game_connection() == _game_connection_film_playback)
        return player->machine_index == 0;
    return 1;
}
