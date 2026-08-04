/* hs_damage_new_evaluate @0x837277D8 — HaloScript builtin evaluator. Argument [0] = damage-effect definition
 * (long), [4] = cutscene flag index (short). No return value. */
#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void hs_damage_new(int definition_index, int16_t flag_index);
void hs_damage_new_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments ) { hs_damage_new(arguments[0], ((__int16 *)arguments)[2] /* +4: flag index (packed HS arg buffer) */); hs_return(thread_index, 0); }
}
