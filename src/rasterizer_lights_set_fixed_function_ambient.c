/* rasterizer_lights_set_fixed_function_ambient @0x838026A0 — set the fixed-function ambient light
 * color to an opaque grayscale value built from one intensity level (level replicated into A/R/G/B). */

#include "headers/rasterizer_globals.h"

void rasterizer_lights_set_fixed_function_ambient(int level)
{
    rasterizer_globals.fixed_function_ambient =
        (((level << 8) & 0xFF00 | 0xFFFF0000 | (unsigned char)level) << 8) | (unsigned char)level;
}
