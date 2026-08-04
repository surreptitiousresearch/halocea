/* ai_scripting_follow_distance_evaluate @0x8372AC90 — HaloScript builtin (ai_follow_distance) evaluator.
 * Argument block: [0] = AI index (long), [4] = follow distance (real). No return value. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_follow_distance(int ai_index, float follow_target_distance);

void ai_scripting_follow_distance_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        ai_scripting_follow_distance(arguments[0], ((float *)arguments)[1]);
        hs_return(thread_index, 0);
    }
}
