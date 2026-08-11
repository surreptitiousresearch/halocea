/* ai_scripting_link_activation_evaluate @0x8372AD70 — HaloScript builtin (ai_link_activation) evaluator.
 * Argument block: [0] = target AI index (long), [4] = link AI index (long). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(int thread_index, int value);
extern void ai_scripting_link_activation(int target_ai_index, int link_ai_index);

void ai_scripting_link_activation_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_link_activation(arguments[0], arguments[1]);
        hs_return(thread_index, 0);
    }
}
