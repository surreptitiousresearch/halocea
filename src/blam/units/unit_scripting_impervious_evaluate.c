/* unit_scripting_impervious_evaluate @0x83729008 — HaloScript builtin wrapper for unit_scripting_impervious. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void unit_scripting_impervious(int object_list_index, uint8_t impervious);

void unit_scripting_impervious_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* HS packed arg block: byte +4 is the boolean second argument (BE high byte of arguments[1]) */
        unit_scripting_impervious(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
