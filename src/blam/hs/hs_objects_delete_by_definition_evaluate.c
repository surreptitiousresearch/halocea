/* hs_objects_delete_by_definition_evaluate @0x83727938 — HaloScript builtin evaluator. Argument [0] = long. No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void hs_objects_delete_by_definition(int definition_index);
void hs_objects_delete_by_definition_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_objects_delete_by_definition(arguments[0]); hs_return(thread_index, 0); }
}
