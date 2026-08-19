/* game_team_is_ally @0x837453D8 — true if our_team is allied with other_team, per the 10x10 ally bit-matrix
 * in game_allegiance_globals (one bit per ordered team pair). Both teams must be in [0,10). */

#include "headers/game_allegiance.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"
#include <stdint.h>

/* attested uint8_t: 6/6 callers byte-normalize (clrlwi 24 / stb) */
uint8_t game_team_is_ally(int16_t our_team, int16_t other_team)
{
    if ( our_team >= 0 && our_team < 10 && other_team >= 0 && other_team < 10 )
    {
        int bit = 10 * our_team + other_team;
        return BIT_VECTOR_TEST_FLAG(game_allegiance_globals->ally_bitvector, bit);
    }
    return 0;
}
