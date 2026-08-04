/* terminator_scoring_rules @0x83816970 — true when the active oddball variant's ball type is terminator. */

#include <stdint.h>
#include "headers/game_variant.h"

extern game_variant *game_engine_get_variant(void);

uint8_t terminator_scoring_rules(void)
{
    return game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type == _oddball_terminator;
}
