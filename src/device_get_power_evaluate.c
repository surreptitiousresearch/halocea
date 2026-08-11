/* device_get_power_evaluate @0x83729288 — HaloScript builtin evaluator. Argument block: [0] = index. Returns a float
 * (re-interpreted into the result word). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern float device_get_power(int device_index);

void device_get_power_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result;
        *(float *)&result = device_get_power(arguments[0]);
        hs_return(thread_index, result);
    }
}
