/* object_pvs_clear_evaluate @0x83728248 — HaloScript builtin evaluator. No arguments; clears the PVS override.
 *
 * DEVIATION: the decompiler shows the engine call with leftover-register arguments; object_pvs_clear takes none. */
#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void object_pvs_clear(void);
void object_pvs_clear_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    object_pvs_clear();
    hs_return(thread_index, 0);
}
