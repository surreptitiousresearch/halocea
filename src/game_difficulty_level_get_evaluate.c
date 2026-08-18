/* game_difficulty_level_get_evaluate @0x8372B6F8 — HaloScript builtin evaluator; no arguments. DEVIATION: `sth r3,
 * 0x50(r1)` @0x8372B718 repeats the word-zeroing store's displacement — the short is the slot's FIRST halfword, not +2. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern int16_t game_difficulty_level_get(void);

void game_difficulty_level_get_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    /* DEVIATION: endian-portable respelling of the BE high-halfword store (was an *(narrow*)&result pun; hs extracts value >> 16) 2026-08-18 */
    int result = (int)((uint16_t)game_difficulty_level_get()) << 16;
    hs_return(thread_index, result);
}
