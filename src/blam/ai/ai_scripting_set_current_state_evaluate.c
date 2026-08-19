/* ai_scripting_set_current_state_evaluate @0x8372A7C8 — HaloScript builtin (ai_set_current_state)
 * evaluator. Argument block: [0] = AI index (long), [4] = state (short/enum). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_set_current_state(int ai_index, int16_t current_state);

void ai_scripting_set_current_state_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_set_current_state(arguments[0], ((int16_t *)arguments)[2]);
        hs_return(thread_index, 0);
    }
}
