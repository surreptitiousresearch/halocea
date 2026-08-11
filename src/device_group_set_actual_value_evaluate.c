/* device_group_set_actual_value_evaluate @0x83729478 — HaloScript builtin evaluator. Argument block:
 * [0] = device group index (short), [4] = value (float). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void device_group_set_actual_value(int16_t group_index, float value);

void device_group_set_actual_value_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        device_group_set_actual_value(*(int16_t *)arguments, ((float *)arguments)[1]);
        hs_return(thread_index, 0);
    }
}
