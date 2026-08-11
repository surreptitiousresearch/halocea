/* ai_scripting_braindead_by_unit_evaluate @0x8372A300 — HaloScript builtin (ai_braindead_by_unit) evaluator.
 * Argument block: [0] = object-list index (int), [4] = braindead (boolean byte). */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_braindead_by_unit(int object_list_index, uint8_t braindead);

void ai_scripting_braindead_by_unit_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_braindead_by_unit(arguments[0], ((unsigned char *)arguments)[4]);
        hs_return(thread_index, 0);
    }
}
