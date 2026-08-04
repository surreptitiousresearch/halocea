/* hs_damage_object_evaluate @0x83727828 — HaloScript builtin evaluator. Argument [0] = damage-effect
 * definition (long), [4] = object index (long). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hs_damage_object(int definition_index, int object_index);
void hs_damage_object_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_damage_object(arguments[0], arguments[1]); hs_return(thread_index, 0); }
}
