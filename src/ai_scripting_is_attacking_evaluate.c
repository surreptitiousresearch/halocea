/* ai_scripting_is_attacking_evaluate @0x8372AF00 — HaloScript builtin (ai_is_attacking) evaluator.
 * Argument block: [0] = AI index (long). Returns the boolean in the result's high byte. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern uint8_t ai_scripting_is_attacking(int ai_index);

void ai_scripting_is_attacking_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        /* DEVIATION: endian-portable respelling of the BE high-byte store (was an *(narrow*)&result pun; hs extracts value >> 24) 2026-08-18 */
        int result = (int)((uint8_t)ai_scripting_is_attacking(arguments[0])) << 24;
        hs_return(thread_index, result);
    }
}
