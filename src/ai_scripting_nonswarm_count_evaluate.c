/* ai_scripting_nonswarm_count_evaluate @0x8372B158 — HaloScript builtin (ai_nonswarm_count) evaluator.
 * Argument block: [0] = AI index (long). Returns the count in the result's high word. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern int ai_scripting_nonswarm_count(int ai_index);

void ai_scripting_nonswarm_count_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        int result = 0;   /* LOWORD = 0 */
        *(int16_t *)&result = ai_scripting_nonswarm_count(arguments[0]);
        hs_return(thread_index, result);
    }
}
