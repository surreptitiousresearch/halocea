/* ai_scripting_magically_see_units_evaluate @0x83729DE8 — HaloScript builtin (ai_magically_see_units)
 * evaluator. Argument block: [0] = AI index (long), [4] = object list index (long). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_magically_see_units(int ai_index, int object_list_index);

void ai_scripting_magically_see_units_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_magically_see_units(arguments[0], arguments[1]);
        hs_return(thread_index, 0);
    }
}
