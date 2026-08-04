/* ai_scripting_reconnect_evaluate @0x8372A868 — HaloScript builtin (ai_reconnect) evaluator. Delegates to
 * the (no-op) engine routine and returns nothing. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void ai_scripting_reconnect(int16_t function_index, int thread_index, uint8_t initialize);

void ai_scripting_reconnect_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    ai_scripting_reconnect(function_index, thread_index, initialize);
    hs_return(thread_index, 0);
}
