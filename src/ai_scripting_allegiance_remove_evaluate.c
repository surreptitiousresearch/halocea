/* ai_scripting_allegiance_remove_evaluate @0x8372A178 — HaloScript builtin (ai_allegiance_remove) evaluator.
 * Argument block: [0] = team1 (short), [4] = team2 (short), each in a 4-byte slot. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_allegiance_remove(int16_t team1_index, int16_t team2_index);

void ai_scripting_allegiance_remove_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_allegiance_remove(arguments[0], arguments[2]);
        hs_return(thread_index, 0);
    }
}
