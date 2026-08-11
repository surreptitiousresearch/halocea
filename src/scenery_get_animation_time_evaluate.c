/* scenery_get_animation_time_evaluate @0x83728320 — HaloScript builtin wrapper; (object) returning a short. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern int16_t scenery_get_animation_time(int scenery_index);

void scenery_get_animation_time_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;   /* DEVIATION: `stw r11, 0x50(r1)` @0x83728338 zeroes the WHOLE word — the halfword */
    /* form left slot bytes 2-3 uninitialised; `sth 0x50(r1)` @0x83728350 puts the short at slot halfword 0. */
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *(int16_t *)&result = scenery_get_animation_time(*arguments);
        hs_return(thread_index, result);
    }
}
