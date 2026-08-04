/* ai_debug_sound_point_set_evaluate @0x8372BF38 — HaloScript builtin evaluator. Debug/retail stub: ignores its
 * arguments and returns void (the underlying debug action is compiled out of this build). */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);

void ai_debug_sound_point_set_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_return(thread_index, 0);
}
