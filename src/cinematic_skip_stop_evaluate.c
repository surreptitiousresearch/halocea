/* cinematic_skip_stop_evaluate @0x8372C118 — HaloScript builtin evaluator. Takes no script arguments; invokes the engine
 * routine and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; cinematic_skip_stop actually
 * takes none, so it is called with its real signature. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void cinematic_skip_stop(void);

void cinematic_skip_stop_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    cinematic_skip_stop();
    hs_return(thread_index, 0);
}
