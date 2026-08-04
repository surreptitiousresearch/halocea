/* hs_effect_new_from_object_marker_evaluate @0x83727788 — HaloScript builtin evaluator. Argument
 * [0] = effect definition (long), [4] = object index (long), [8] = marker name (string). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hs_effect_new_from_object_marker(int definition_index, int object_index, const char *marker_name);
void hs_effect_new_from_object_marker_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        hs_effect_new_from_object_marker(arguments[0], arguments[1], (const char *)arguments[2]);
        hs_return(thread_index, 0);
    }
}
