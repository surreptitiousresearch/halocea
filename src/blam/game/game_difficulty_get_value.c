/* game_difficulty_get_value @0x837B6768 — sample a difficulty-scaled tuning value from the globals tag's
 * difficulty_information block. value_type selects the row; the current difficulty (clamped 0..3) selects
 * the column. Returns 1.0 if the globals tag or its difficulty block is absent. */

#include <stdint.h>
#include "headers/game_globals_tag.h"
#include "headers/game_globals_difficulty_information.h"
#include "headers/game_difficulty.h"

extern int16_t game_difficulty_level_get(void);

extern float game_difficulty_get_value_by_difficulty(int16_t value_type, int16_t difficulty);
float game_difficulty_get_value(int16_t value_type)
{
    /* DEVIATION: inlined body of game_difficulty_get_value_by_difficulty@0x837B66F8 (zero-xref donor); donor's `difficulty` param folds to the game_difficulty_level_get() call already made here — collapsed to a direct call */
    return game_difficulty_get_value_by_difficulty(value_type, game_difficulty_level_get());
}
