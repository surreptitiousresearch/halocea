/* ai_scripting_defend_evaluate @0x83729F10 — HaloScript builtin (ai_defend) evaluator.
 * Argument block: [0] = AI index (long). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_defend(int ai_index);

void ai_scripting_defend_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_defend(arguments[0]);
        hs_return(thread_index, 0);
    }
}
