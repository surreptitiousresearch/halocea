/* object_list_from_ai_reference_evaluate @0x8372B1B0 — HaloScript builtin evaluator. Argument [0] = AI index
 * (long). Returns a new object-list reference of the referenced units. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern int object_list_from_ai_reference(int ai_index);
void object_list_from_ai_reference_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_return(thread_index, object_list_from_ai_reference(arguments[0])); }
}
