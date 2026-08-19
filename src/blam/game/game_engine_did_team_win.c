/* game_engine_did_team_win @0x8374E738 — find the first active player on `team_index` (player record dword
 * +8, i.e. +32 bytes) and defer to game_engine_did_player_win for that player's index. False if no player
 * is on that team.
 *
 * DEVIATION: the decompiler inlined game_engine_did_player_win's own game_engine-callback-or-default
 * fallback verbatim; restored as a call to the already-recovered game_engine_did_player_win.c, which is
 * byte-for-byte the same logic. */

#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern int game_engine_did_player_win(int player_index);

int game_engine_did_team_win(int team_index)
{
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return 0;

    while ( player->team_index != team_index )
    {
        player = data_iterator_next(&iterator);
        if ( !player )
            return 0;
    }

    return game_engine_did_player_win(iterator.index);
}
