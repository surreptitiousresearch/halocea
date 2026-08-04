/* rasterizer_lights_set_fixed_function_ambient_evaluate @0x8372D9C8 — HaloScript builtin wrapper for
 * rasterizer_lights_set_fixed_function_ambient. */

#include <stdint.h>

extern int * hs_macro_function_evaluate(int16_t function_index, int thread_index, uint8_t initialize);
extern void hs_return(uint16_t thread_index, int value);
extern void rasterizer_lights_set_fixed_function_ambient(int level);

void rasterizer_lights_set_fixed_function_ambient_evaluate(int16_t function_index, int thread_index, uint8_t initialize)
{
    int *arguments = hs_macro_function_evaluate(function_index, thread_index, initialize);
    if ( arguments )
    {
        rasterizer_lights_set_fixed_function_ambient(*arguments);
        hs_return(thread_index, 0);
    }
}
