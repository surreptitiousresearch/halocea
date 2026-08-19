/* game_difficulty_level_get_ignore_easy_evaluate @0x8372B6B0 — HaloScript builtin evaluator. Takes no arguments; returns a short packed into
 * the high word of the result (low word zero). */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern int16_t game_difficulty_level_get_ignore_easy(void);

void game_difficulty_level_get_ignore_easy_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    /* DEVIATION: endian-portable respelling of the BE high-halfword store (was an *(narrow*)&result pun; hs extracts value >> 16) 2026-08-18 */
    int result = (int)((uint16_t)game_difficulty_level_get_ignore_easy()) << 16;
    hs_return(thread_index, result);
}
