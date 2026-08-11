/* recorded_animation_kill_evaluate @0x83727D50 — HaloScript builtin wrapper for recorded_animation_kill; (object). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void recorded_animation_kill(int unit_index);

void recorded_animation_kill_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        recorded_animation_kill(*arguments);
        hs_return(thread_index, 0);
    }
}
