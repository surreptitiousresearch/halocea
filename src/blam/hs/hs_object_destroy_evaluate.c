/* hs_object_destroy_evaluate @0x837273B0 — HaloScript builtin evaluator. Argument [0] = object index (long).
 * No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void hs_object_destroy(int object_index);
void hs_object_destroy_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_object_destroy(arguments[0]); hs_return(thread_index, 0); }
}
