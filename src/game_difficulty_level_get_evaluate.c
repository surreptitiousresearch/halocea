/* game_difficulty_level_get_evaluate @0x8372B6F8 — HaloScript builtin evaluator. Takes no arguments; returns a short packed into
 * the high word of the result (low word zero). */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern int16_t game_difficulty_level_get(void);

void game_difficulty_level_get_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    *((int16_t *)&result + 1) = game_difficulty_level_get();
    hs_return(thread_index, result);
}
