/* ai_scripting_allegiance_broken_evaluate @0x8372B358 — HaloScript builtin (ai_allegiance_broken) evaluator.
 * Argument block: [0] = team1 (short), [4] = team2 (short). Returns the boolean in the result's high byte. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t ai_scripting_allegiance_broken(int16_t team1_index, int16_t team2_index);

void ai_scripting_allegiance_broken_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    __int16 *arguments = (__int16 *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result = 0;
        *((unsigned char *)&result + 3) = ai_scripting_allegiance_broken(arguments[0], arguments[2]);
        hs_return(thread_index, result);
    }
}
