/* game_safe_to_save_evaluate @0x8372C2F8 — HaloScript builtin evaluator. Takes no arguments; returns a boolean packed into
 * the high byte of the result word. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern uint8_t game_safe_to_save(void);

void game_safe_to_save_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs extracts value >> 24) 2026-08-18 */
    int result = (int)((uint8_t)game_safe_to_save()) << 24;
    hs_return(thread_index, result);
}
