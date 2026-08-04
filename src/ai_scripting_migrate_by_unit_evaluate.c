/* ai_scripting_migrate_by_unit_evaluate @0x8372A0D8 — HaloScript builtin (ai_migrate_by_unit) evaluator.
 * Argument block: [0] = object list index (long), [4] = target AI index (long). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_migrate_by_unit(int object_list_index, int ai_index);

void ai_scripting_migrate_by_unit_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_migrate_by_unit(arguments[0], arguments[1]);
        hs_return(thread_index, 0);
    }
}
