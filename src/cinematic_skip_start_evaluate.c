/* cinematic_skip_start_evaluate @0x8372C0E0 — HaloScript builtin evaluator. Takes no script arguments; invokes the engine
 * routine and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; cinematic_skip_start actually
 * takes none, so it is called with its real signature. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void cinematic_skip_start(void);

void cinematic_skip_start_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    cinematic_skip_start();
    hs_return(thread_index, 0);
}
