/* ai_scripting_look_at_object_evaluate @0x8372AA78 — HaloScript builtin (ai_look_at_object) evaluator.
 * Argument block: [0] = unit index (long), [4] = object index (long). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_look_at_object(int unit_index, int object_index);

void ai_scripting_look_at_object_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_look_at_object(arguments[0], arguments[1]);
        hs_return(thread_index, 0);
    }
}
