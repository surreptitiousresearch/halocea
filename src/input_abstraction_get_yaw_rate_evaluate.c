/* input_abstraction_get_yaw_rate_evaluate @0x8372E010 — HaloScript builtin wrapper for input_abstraction_get_yaw_rate; (short) returning a real. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern float input_abstraction_get_yaw_rate(int16_t local_player_index);

void input_abstraction_get_yaw_rate_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result;
        *(float *)&result = input_abstraction_get_yaw_rate(*(__int16 *)arguments);
        hs_return(thread_index, result);
    }
}
