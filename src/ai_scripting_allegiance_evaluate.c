/* ai_scripting_allegiance_evaluate @0x8372A128 — HaloScript builtin (ai_allegiance) evaluator. Argument block:
 * [0] = team1 (short), [4] = team2 (short), each in a 4-byte slot. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_allegiance(int16_t team1_index, int16_t team2_index);

void ai_scripting_allegiance_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    __int16 *arguments = (__int16 *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_allegiance(arguments[0], arguments[2]);
        hs_return(thread_index, 0);
    }
}
