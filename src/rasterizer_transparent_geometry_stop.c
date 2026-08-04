/* rasterizer_transparent_geometry_stop @0x83768680 — tail-call: reset the stencil mode to 0. */

#include <stdint.h>

extern void rasterizer_set_stencil_mode(int16_t stencil_mode);

void rasterizer_transparent_geometry_stop(void)
{
    rasterizer_set_stencil_mode(0);
}
