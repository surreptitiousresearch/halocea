/* device_set_never_appears_locked_evaluate @0x837291E8 — HaloScript builtin evaluator. Argument block: [0] = device index (long),
 * [4] = boolean. No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void device_set_never_appears_locked(int device_index, uint8_t never_appears_locked);

void device_set_never_appears_locked_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        device_set_never_appears_locked(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
