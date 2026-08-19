/* ai_scripting_allegiance_broken_evaluate @0x8372B358 — HaloScript builtin (ai_allegiance_broken) evaluator.
 * Argument block: [0] = team1 (short), [4] = team2 (short). Returns the boolean in the result's high byte. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t ai_scripting_allegiance_broken(int16_t team1_index, int16_t team2_index);

void ai_scripting_allegiance_broken_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int16_t *arguments = (int16_t *)hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs extracts value >> 24) 2026-08-18 */
        int result = (int)((uint8_t)ai_scripting_allegiance_broken(arguments[0], arguments[2])) << 24;
        hs_return(thread_index, result);
    }
}
