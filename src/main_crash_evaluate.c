/* main_crash_evaluate @0x8372BD30 — HaloScript wrapper for (main_crash <string>). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void main_crash(const char *str);
extern void hs_return(uint16_t thread_index, int value);

void main_crash_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    const char **str = (const char **)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( str )
    {
        main_crash(*str);
        hs_return(thread_index, 0);
    }
}
