/* game_engine_player_added @0x83750420 — initialise per-player engine state when a player joins. Resets
 * the player's multiplayer info (speed multiplier = 1.0, teleporter/state-message/display fields = -1)
 * and zeroes the first two multiplayer statistics words. On the server it assigns a team: alternating
 * teams for a teamed variant, or the player's preferred team otherwise. Finally it broadcasts a join
 * message for the player to every player and fires the engine's player_added callback.
 *
 * DEVIATION: the decompiler renders the -1 index stores as float NAN; the disassembly shows plain `stw`
 * of -1 (the "NONE" sentinel), modelled as int here. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/game_variant.h"
#include "headers/game_engine_globals.h"
#include "headers/game_engine.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/game_engine_message.h"
#include "headers/blam_data_globals.h"

extern struct game_engine    *game_engine;

extern int16_t game_connection(void);
extern void multiplayer_message(int recipient_player_index, int subject_player_index, int message, int message_data, uint8_t should_replicate);

extern void initialize_player_multiplayer_data(int player_index);
void game_engine_player_added(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    /* DEVIATION: collapsed verbatim-inlined body of initialize_player_multiplayer_data@0x83746008 (zero-xref donor) into a call; host already holds player_index so no constant-folding of donor args was needed — the local `player` pointer (DATA_ARRAY_ELEMENT lookup on the preceding line) is kept because it is still used later in this function for team-index assignment. */
    initialize_player_multiplayer_data(player_index);

    if (!game_engine)
        return;

    if (game_connection() != _game_connection_network_client)
    {
        if (global_variant.universal_variant.teams)
        {
            int16_t connection = game_connection();
            if (connection == _game_connection_network_client || connection == _game_connection_network_server)
            {
                player->team_index = player->network_player_data.team_index % 2;
            }
            else
            {
                int team = (signed char)game_engine_globals.current_team_to_add;
                player->network_player_data.team_index = game_engine_globals.current_team_to_add;
                player->team_index = team;
                game_engine_globals.current_team_to_add = (game_engine_globals.current_team_to_add + 1) % 2;
            }
        }
        else
        {
            char preferred_team = player->network_player_data.player_list_index;   /* DB name; used as preferred team */
            player->team_index = preferred_team;
            player->network_player_data.team_index = preferred_team;
            ++game_engine_globals.current_team_to_add;
        }
    }

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    while (data_iterator_next(&iterator))
    {
        int subject_player_index = (player_index == -1) ? iterator.index : player_index;
        multiplayer_message(iterator.index, subject_player_index, game_engine_message_welcome, -1, 0);
    }

    void (*player_added)(int) = game_engine->player_added;
    if (player_added)
        player_added(player_index);
}
