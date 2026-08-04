/* hs_object_destroy_all_evaluate @0x83727518 — HaloScript builtin evaluator. Takes no arguments; invokes the
 * engine routine and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; hs_object_destroy_all
 * actually takes none. */
#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void hs_object_destroy_all(void);
void hs_object_destroy_all_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    hs_object_destroy_all();
    hs_return(thread_index, 0);
}
