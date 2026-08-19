/* game_engine_player_is_out_of_lives @0x83747370 — true when a finite-lives variant is active, the
 * player slot is unoccupied by a living unit (object index -1), and the player's death count has reached
 * the variant's life limit. Players are never out of lives in an unlimited-lives variant. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_variant.h"
#include "headers/blam_data_globals.h"


uint8_t game_engine_player_is_out_of_lives(int player_index)
{
    if (global_variant.universal_variant.lives <= 0)
        return 0;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    /* unit_index == -1 means dead/awaiting respawn */
    if (player->unit_index != -1)
        return 0;

    return player->statistics.deaths >= global_variant.universal_variant.lives;
}
