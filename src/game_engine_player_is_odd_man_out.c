/* game_engine_player_is_odd_man_out @0x8374727C — in an "odd man out" variant, decide whether a dead,
 * awaiting-respawn player is the lone survivor's target (the single odd man). False unless the variant is
 * odd-man-out and the player is currently dead. On a client the server-computed cached flag
 * (multiplayer.is_odd_man_out) is trusted. On the server a player that has never died is never the odd
 * man; otherwise the player is the odd man only if no other dead player outranks it (later time of
 * death, or equal time with a lower player index). The result is cached in multiplayer.is_odd_man_out. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/game_variant.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern int16_t game_connection(void);

uint8_t game_engine_player_is_odd_man_out(int player_index)
{
    int player_slot = (unsigned __int16)player_index;
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    /* unit_index == -1 means dead/awaiting respawn */
    if (!global_variant.universal_variant.odd_man_out || player->unit_index != -1)
        return 0;

    if (game_connection() == _game_connection_network_client)
        return player->multiplayer.is_odd_man_out;

    /* statistics.deaths <= 0: a deathless player is never the odd man */
    if (player->statistics.deaths <= 0)
    {
        player->multiplayer.is_odd_man_out = 0;
        return 0;
    }

    int is_odd_man = 1;
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);

    for (player_datum *other = data_iterator_next(&iterator); other;
         other = data_iterator_next(&iterator))
    {
        if (other->unit_index == -1 && other != player)
        {
            /* offset 132 = multiplayer.time_of_death (not a score): later death outranks */
            int other_score = other->multiplayer.time_of_death;
            int my_score    = player->multiplayer.time_of_death;
            if (other_score > my_score ||
                (other_score == my_score && player_slot < (unsigned __int16)iterator.index))
                is_odd_man = 0;
        }
    }

    player->multiplayer.is_odd_man_out = is_odd_man;
    return is_odd_man;
}
