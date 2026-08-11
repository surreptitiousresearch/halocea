/* unit_scripting_unit_driver_evaluate @0x83728D88 — HaloScript builtin wrapper for unit_scripting_unit_driver; returns an object index. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern int unit_scripting_unit_driver(int unit_index);

void unit_scripting_unit_driver_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
        hs_return(thread_index, unit_scripting_unit_driver(arguments[0]));
}
