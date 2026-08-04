/* profile_initialize_evaluate @0x8372BED8 — HaloScript builtin stub; performs no work and returns void. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);

void profile_initialize_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_return(thread_index, 0);
}
