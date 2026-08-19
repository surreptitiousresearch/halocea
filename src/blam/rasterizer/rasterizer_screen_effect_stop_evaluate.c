/* rasterizer_screen_effect_stop_evaluate @0x8372DCB8 — HaloScript builtin wrapper.
 *
 * DEVIATION: rasterizer_screen_effect_stop is a void no-argument builtin; the decompiler renders it as taking
 * the evaluator (function_index, thread_index, initialize) triple, so it is called that way here. */

#include <stdint.h>

extern void hs_return(int thread_index, int value);
extern void rasterizer_screen_effect_stop(void);

void rasterizer_screen_effect_stop_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    rasterizer_screen_effect_stop();
    hs_return(thread_index, 0);
}
