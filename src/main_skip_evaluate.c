/* main_skip_evaluate @0x8372C688 — HaloScript wrapper for (main_skip <short>). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void main_skip(int16_t ticks);
extern void hs_return(uint16_t thread_index, int value);

void main_skip_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    __int16 *ticks = (__int16 *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( ticks )
    {
        main_skip(*ticks);
        hs_return(thread_index, 0);
    }
}
