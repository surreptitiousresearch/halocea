/* oddball_test_trait @0x83816A0C — tests whether `trait` (a game_engine_trait id) matches the active
 * oddball variant's trait-with-ball (if the player currently has the ball) or trait-without-ball. A trait
 * id of 0 (none) never matches. */

#include <stdint.h>
#include "headers/game_variant.h"

extern game_variant *game_engine_get_variant(void);
extern uint8_t player_has_ball(int player_index);

uint8_t oddball_test_trait(int player_index, int trait)
{
    if ( !trait )
        return 0;

    if ( player_has_ball(player_index) )
        return game_engine_get_variant()->game_engine_variant.oddball.trait_with_ball == trait;

    return game_engine_get_variant()->game_engine_variant.oddball.trait_without_ball == trait;
}
