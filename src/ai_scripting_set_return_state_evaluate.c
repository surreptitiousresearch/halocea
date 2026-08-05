/* ai_scripting_set_return_state_evaluate @0x8372A778 — HaloScript builtin (ai_set_return_state)
 * evaluator. Argument block: [0] = AI index (long), [4] = state (short/enum). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_set_return_state(int ai_index, int16_t default_state);

void ai_scripting_set_return_state_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_set_return_state(arguments[0], ((int16_t *)arguments)[2]);
        hs_return(thread_index, 0);
    }
}
