/* objects_dump_memory_evaluate @0x83727E90 — HaloScript builtin evaluator. No arguments; writes the object
 * memory report and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with leftover-register arguments; objects_dump_memory takes
 * none. */
#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void objects_dump_memory(void);
void objects_dump_memory_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    objects_dump_memory();
    hs_return(thread_index, 0);
}
