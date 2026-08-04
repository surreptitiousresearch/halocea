/* game_engine_get_team_score @0x83745FC0 — forwards through the active game_engine's get_team_score
 * callback (indirect tail call via ctr, confirmed via disasm/decompiler). */

#include "headers/game_engine.h"

int game_engine_get_team_score(int team_index)
{
    return game_engine->get_team_score(team_index);
}
