/* ai_scripting_follow_target_ai_evaluate @0x8372AC40 — HaloScript builtin (ai_follow_target_ai) evaluator.
 * Argument block: [0] = AI index (long), [4] = target AI index (long). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_follow_target_ai(int ai_index, int target_ai_index);

void ai_scripting_follow_target_ai_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_follow_target_ai(arguments[0], arguments[1]);
        hs_return(thread_index, 0);
    }
}
