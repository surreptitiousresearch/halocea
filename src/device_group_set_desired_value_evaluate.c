/* device_group_set_desired_value_evaluate @0x83729420 — HaloScript builtin evaluator. Argument block:
 * [0] = device group index (short), [4] = value (float). Returns a boolean (did the value change) packed
 * into the high byte of the result word. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t device_group_set_desired_value(int16_t group_index, float desired_value);

void device_group_set_desired_value_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int result = 0;
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        *((unsigned char *)&result + 3) = device_group_set_desired_value(*(int16_t *)arguments, ((float *)arguments)[1]);
        hs_return(thread_index, result);
    }
}
