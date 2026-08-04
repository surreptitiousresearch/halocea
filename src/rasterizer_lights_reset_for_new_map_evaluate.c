/* rasterizer_lights_reset_for_new_map_evaluate @0x8372DA88 — HaloScript builtin wrapper.
 *
 * DEVIATION: rasterizer_lights_reset_for_new_map is a void no-argument builtin; the decompiler renders it as
 * taking the evaluator (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(uint16_t thread_index, int value);
extern void rasterizer_lights_reset_for_new_map(void);

void rasterizer_lights_reset_for_new_map_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    rasterizer_lights_reset_for_new_map();
    hs_return(thread_index, 0);
}
