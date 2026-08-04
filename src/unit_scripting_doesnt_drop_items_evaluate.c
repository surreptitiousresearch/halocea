/* unit_scripting_doesnt_drop_items_evaluate @0x83728FC0 — HaloScript builtin wrapper. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void unit_scripting_doesnt_drop_items(int object_list_index);

void unit_scripting_doesnt_drop_items_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        unit_scripting_doesnt_drop_items(arguments[0]);
        hs_return(thread_index, 0);
    }
}
