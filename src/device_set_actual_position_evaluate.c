/* device_set_actual_position_evaluate @0x83729380 — HaloScript builtin evaluator. Argument block: [0] = device index (long),
 * [4] = value (float). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void device_set_actual_position(int device_index, float value);

void device_set_actual_position_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        device_set_actual_position(arguments[0], ((float *)arguments)[1]);
        hs_return(thread_index, 0);
    }
}
