/* unit_scripting_suspended_evaluate @0x83729058 — HaloScript builtin wrapper for unit_scripting_suspended. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void unit_scripting_suspended(int unit_index, uint8_t suspend);

void unit_scripting_suspended_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS packed arg block: byte +4 is the boolean second argument (BE high byte of arguments[1]) */
        unit_scripting_suspended(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
