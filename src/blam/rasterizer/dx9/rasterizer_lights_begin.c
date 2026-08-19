/* rasterizer_lights_begin @0x837923B0 — thunk: clears the submitted-light count, then tail-calls the
 * platform implementation _rasterizer_lights_begin. */

#include "headers/rasterizer_lights.h"

extern void _rasterizer_lights_begin(void);

void rasterizer_lights_begin(void)
{
    rasterizer_lights.light_count = 0;
    _rasterizer_lights_begin();
}
