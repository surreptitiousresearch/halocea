/* game_all_quiet_evaluate @0x8372C340 — HaloScript builtin evaluator. Takes no arguments; returns a boolean
 * packed into byte 0 of the 4-byte HaloScript value slot (the word's most significant byte on big-endian PPC). */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern uint8_t game_all_quiet(void);

void game_all_quiet_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    /* DEVIATION: `stb r3, 0x50(r1)` @0x8372C360 carries the SAME displacement as the word-zeroing
     * `stw r11, 0x50(r1)` @0x8372C358 — byte offset +0, not +3. Sub-word HaloScript values occupy the
     * slot's leading bytes (hs_evaluate_if tests `*(char *)condition_result`; hs_evaluate_equality
     * compares slot[0..hs_type_sizes[type]-1]), so the offset-0 lvalue is at once the faithful
     * transcription and the spelling that survives a little-endian port. */
    *(unsigned char *)&result = game_all_quiet();
    hs_return(thread_index, result);
}
