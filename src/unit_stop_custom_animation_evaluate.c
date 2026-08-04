/* unit_stop_custom_animation_evaluate @0x837285F8 — HaloScript builtin wrapper for unit_stop_custom_animation. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void unit_stop_custom_animation(int unit_index);

void unit_stop_custom_animation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        unit_stop_custom_animation(arguments[0]);
        hs_return(thread_index, 0);
    }
}
