/* device_set_power_evaluate @0x83729238 — HaloScript builtin evaluator. Argument block: [0] = device index (long),
 * [4] = value (float). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void device_set_power(int device_index, float power);

void device_set_power_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        device_set_power(arguments[0], ((float *)arguments)[1]);
        hs_return(thread_index, 0);
    }
}
