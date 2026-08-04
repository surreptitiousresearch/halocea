/* ai_scripting_erase_all_evaluate @0x83729B10 — HaloScript builtin (ai_erase_all) evaluator. Delegates to
 * the engine routine and returns nothing. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_erase_all(int16_t function_index, int thread_index, uint8_t initialize);

void ai_scripting_erase_all_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    ai_scripting_erase_all(function_index, thread_index, initialize);
    hs_return(thread_index, 0);
}
