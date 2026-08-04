/* recorded_animation_get_time_left_evaluate @0x83727D98 — HaloScript builtin wrapper; (object) returning a short. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern __int16 recorded_animation_get_time_left(int object_index);

void recorded_animation_get_time_left_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result;
    *((__int16 *)&result) = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *((__int16 *)&result + 1) = recorded_animation_get_time_left(*arguments);
        hs_return(thread_index, result);
    }
}
