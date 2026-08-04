/* game_team_is_enemy @0x83745320 — is other_team hostile to our_team? In a running multiplayer game
 * engine, any two distinct teams are enemies. Otherwise (campaign/AI), consult the allegiance globals'
 * friendly_bitvector: a set bit at index (10*our_team + other_team) means the pair is friendly, so the
 * teams are enemies when that bit is clear. Out-of-range team indices are treated as enemies. */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/game_allegiance.h"

extern uint8_t game_engine_running(void);

uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team)
{
    if ( game_engine_running() )
        return other_team != our_team;

    if ( (unsigned int)our_team > 9 || (unsigned int)other_team > 9 )
        return 1;

    int pair_index = 10 * our_team + other_team;
    return !BIT_VECTOR_TEST_FLAG(game_allegiance_globals->friendly_bitvector, pair_index);
}
