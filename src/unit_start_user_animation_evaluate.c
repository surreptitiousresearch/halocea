/* unit_start_user_animation_evaluate @0x83728640 — HaloScript builtin wrapper; packs a boolean result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t unit_start_user_animation(int unit_index, int animation_graph_index, const char *animation_name, uint8_t interpolate);

void unit_start_user_animation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: `stb r3, 0x50(r1)` @0x8372867C — a BYTE store at the same displacement as the
         * word-zeroing `stw r11, 0x50(r1)` @0x83728650, i.e. slot byte +0. A previous round replaced
         * this with a plain widening `result = ...` on the belief that byte +3 was the faithful
         * transcription; that emits a full-word store and puts the boolean at byte +3 on big-endian
         * PPC — the opposite end from the image. Slot byte 0 is what every consumer reads
         * (hs_evaluate_if's `*(char *)condition_result`), and is correct on x64 as well.
         * HS packed arg block: byte +12 is the boolean interpolate arg — `lbz r6, 0xC(r3)` @0x83728668. */
        *(unsigned char *)&result = unit_start_user_animation(
            arguments[0], arguments[1], (const char *)arguments[2], ((unsigned char *)arguments)[12]);
        hs_return(thread_index, result);
    }
}
