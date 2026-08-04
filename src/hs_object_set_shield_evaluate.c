/* hs_object_set_shield_evaluate @0x837275F0 — HaloScript builtin evaluator. Argument [0] = object index
 * (long), [4] = shield fraction (float). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hs_object_set_shield(int object_index, float shield);
void hs_object_set_shield_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_object_set_shield(arguments[0], ((float *)arguments)[1]); hs_return(thread_index, 0); }
}
