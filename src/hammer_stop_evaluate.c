/* hammer_stop_evaluate @0x8372D0A8 — HaloScript builtin evaluator. Stub: returns void (action compiled out of this
 * build). */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);

void hammer_stop_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_return(thread_index, 0);
}
