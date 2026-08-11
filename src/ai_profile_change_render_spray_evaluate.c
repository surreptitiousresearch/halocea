/* ai_profile_change_render_spray_evaluate @0x8372BF28 — HaloScript builtin evaluator. Debug/retail stub: ignores its
 * arguments and returns void (the underlying debug action is compiled out of this build). */

#include <stdint.h>

extern void hs_return(int thread_index, int value);

void ai_profile_change_render_spray_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_return(thread_index, 0);
}
