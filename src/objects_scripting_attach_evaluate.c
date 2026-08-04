/* objects_scripting_attach_evaluate @0x83727F68 — HaloScript builtin evaluator. Argument [0] = parent object
 * (long), [4] = parent marker (string), [8] = child object (long), [12] = child marker (string). No return
 * value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void objects_scripting_attach(int parent_object_index, const char *parent_marker_name, int child_object_index, const char *child_marker_name);
void objects_scripting_attach_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        objects_scripting_attach(arguments[0], (const char *)arguments[1], arguments[2], (const char *)arguments[3]);
        hs_return(thread_index, 0);
    }
}
