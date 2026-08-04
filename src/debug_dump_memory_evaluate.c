/* debug_dump_memory_evaluate @0x8372BE98 — HaloScript builtin evaluator. Stub: ignores its arguments and returns void
 * (the underlying debug action is compiled out of this build). */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);

void debug_dump_memory_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_return(thread_index, 0);
}
