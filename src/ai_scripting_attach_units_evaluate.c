/* ai_scripting_attach_units_evaluate @0x837298C0 — HaloScript builtin (ai_attach_units list form) evaluator.
 * Argument block: [0] = object-list index (int), [4] = ai index (int). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_attach_units(int object_list_index, unsigned int ai_index);

void ai_scripting_attach_units_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_attach_units(arguments[0], arguments[1]);
        hs_return(thread_index, 0);
    }
}
