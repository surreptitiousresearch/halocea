/* device_group_get_value_evaluate @0x837293D0 — HaloScript builtin evaluator. Argument block: [0] = index. Returns a float
 * (re-interpreted into the result word). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern float device_group_get_value(int group_index);

void device_group_get_value_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result;
        *(float *)&result = device_group_get_value(*(__int16 *)arguments);
        hs_return(thread_index, result);
    }
}
