/* unit_get_custom_animation_time_evaluate @0x837285A0 — HaloScript builtin wrapper; packs a short result. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern int16_t unit_get_custom_animation_time(int unit_index);

void unit_get_custom_animation_time_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result;
    *((int16_t *)&result) = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *((int16_t *)&result + 1) = unit_get_custom_animation_time(arguments[0]);
        hs_return(thread_index, result);
    }
}
