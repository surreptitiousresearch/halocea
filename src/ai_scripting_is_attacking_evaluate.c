/* ai_scripting_is_attacking_evaluate @0x8372AF00 — HaloScript builtin (ai_is_attacking) evaluator.
 * Argument block: [0] = AI index (long). Returns the boolean in the result's high byte. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern uint8_t ai_scripting_is_attacking(int ai_index);

void ai_scripting_is_attacking_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result = 0;
        *((unsigned char *)&result + 3) = ai_scripting_is_attacking(arguments[0]);
        hs_return(thread_index, result);
    }
}
