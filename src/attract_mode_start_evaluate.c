/* attract_mode_start_evaluate @0x8372C278 — HaloScript builtin evaluator. Stub: returns void (attract-mode
 * start is handled outside the script engine / compiled out of this build). */

#include <stdint.h>

extern void hs_return(int thread_index, int value);

void attract_mode_start_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_return(thread_index, 0);
}
