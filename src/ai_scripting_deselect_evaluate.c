/* ai_scripting_deselect_evaluate @0x83729B90 — HaloScript builtin (ai_deselect) evaluator. Delegates to the
 * (no-op) engine routine and returns nothing. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void ai_scripting_deselect(int16_t function_index, int thread_index, uint8_t initialize);

void ai_scripting_deselect_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    ai_scripting_deselect(function_index, thread_index, initialize);
    hs_return(thread_index, 0);
}
