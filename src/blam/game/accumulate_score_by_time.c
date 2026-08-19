/* accumulate_score_by_time @0x83816940 — true unless the active oddball variant is terminator (which
 * scores by kills, not elapsed carry time).
 *
 * DEVIATION: the decompiler renders this as
 *   (t-2) - ((t-3) + (t==_oddball_terminator))
 * which algebraically reduces to `1 - (t==_oddball_terminator)` for any t; reproduced as the equivalent
 * boolean expression rather than the raw arithmetic. */

#include "headers/game_variant.h"

extern game_variant *game_engine_get_variant(void);

int accumulate_score_by_time(void)
{
    return game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type != _oddball_terminator;
}
