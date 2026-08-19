/* recorded_animation_play_and_hover_evaluate @0x83727CF8 — HaloScript builtin wrapper for recorded_animation_play_and_hover; (object, short) returning bool. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t recorded_animation_play_and_hover(int vehicle_index, int16_t animation_index);

void recorded_animation_play_and_hover_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: packed wire, faithfully de-flagged */
        /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs_inspect_boolean extracts value >> 24) 2026-08-18 */
        int result = (int)((uint8_t)(recorded_animation_play_and_hover(arguments[0], ((int16_t *)arguments)[2]))) << 24;
        hs_return(thread_index, result);
    }
}
