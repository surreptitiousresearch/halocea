/* recorded_animation_play_evaluate @0x83727C48 — HaloScript builtin wrapper for recorded_animation_play; (object, short) returning bool. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t recorded_animation_play(int unit_index, int16_t animation_index);

void recorded_animation_play_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS argument block: packed wire, faithfully de-flagged */
        *((unsigned char *)&result + 3) = recorded_animation_play(arguments[0], ((int16_t *)arguments)[2]);
        hs_return(thread_index, result);
    }
}
