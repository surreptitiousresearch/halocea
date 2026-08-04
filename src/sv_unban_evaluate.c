/* sv_unban_evaluate @0x8372E6C0 — HaloScript builtin wrapper for the server command sv_unban. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void sv_unban(int index);

void sv_unban_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        sv_unban(*arguments);
        hs_return(thread_index, 0);
    }
}
