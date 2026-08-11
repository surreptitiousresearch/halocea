/* hs_object_set_facing_evaluate @0x837275A0 — HaloScript builtin evaluator. Argument [0] = object index (long),
 * [4] = scenario flag index (short). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void hs_object_set_facing(int object_index, int16_t flag_index);
void hs_object_set_facing_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_object_set_facing(arguments[0], ((int16_t *)arguments)[2]); hs_return(thread_index, 0); }
}
