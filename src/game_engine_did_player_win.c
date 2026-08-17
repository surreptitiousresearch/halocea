/* game_engine_did_player_win @0x8374E700 */
#include "headers/game_engine.h"

extern int game_engine_did_player_win_default(int player_index);

int game_engine_did_player_win(int player_index)
{
    if ( !game_engine )
        return 0;
    if ( game_engine->game_engine_did_player_win )
        return game_engine->game_engine_did_player_win(player_index);
    return game_engine_did_player_win_default(player_index);
}
