/* unit_kill_evaluate @0x83728510 — HaloScript builtin wrapper for unit_kill. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void unit_kill(int unit_index);

void unit_kill_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        unit_kill(arguments[0]);
        hs_return(thread_index, 0);
    }
}
