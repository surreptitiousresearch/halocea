/* director_load_camera_evaluate @0x8372B5F8 — HaloScript builtin evaluator. Takes no arguments; invokes the engine routine
 * and returns void.
 *
 * DEVIATION: the decompiler shows the engine call with three leftover-register arguments; director_load_camera actually
 * takes none, so it is called with its real signature. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void director_load_camera(void);

void director_load_camera_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    director_load_camera();
    hs_return(thread_index, 0);
}
