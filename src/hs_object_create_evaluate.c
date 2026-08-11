/* hs_object_create_evaluate @0x83727368 — HaloScript builtin evaluator. Argument [0] = object name index
 * (short). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void hs_object_create(int16_t object_name_index);
void hs_object_create_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_object_create(*arguments); hs_return(thread_index, 0); }
}
