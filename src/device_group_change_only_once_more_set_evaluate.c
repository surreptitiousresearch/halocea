/* device_group_change_only_once_more_set_evaluate @0x83729568 — HaloScript builtin evaluator. Argument
 * block: [0] = device group index (short), [4] = boolean. No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void device_group_change_only_once_more_set(int device_group_index, uint8_t change_only_once);

void device_group_change_only_once_more_set_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        device_group_change_only_once_more_set(*(__int16 *)arguments, ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
