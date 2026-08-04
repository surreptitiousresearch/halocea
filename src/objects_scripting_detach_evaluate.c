/* objects_scripting_detach_evaluate @0x83727FC0 — HaloScript builtin evaluator. Argument [0] = parent object
 * (long), [4] = child object (long). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void objects_scripting_detach(int parent_object_index, int child_object_index);
void objects_scripting_detach_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { objects_scripting_detach(arguments[0], arguments[1]); hs_return(thread_index, 0); }
}
