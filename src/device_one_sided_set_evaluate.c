/* device_one_sided_set_evaluate @0x837294C8 — HaloScript builtin evaluator. Argument block: [0] = device index (long),
 * [4] = boolean. No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void device_one_sided_set(int device_index, uint8_t one_sided);

void device_one_sided_set_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        device_one_sided_set(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
