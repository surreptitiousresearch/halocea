/* rasterizer_decals_begin @0x836A60D8 — tail-call thunk forwarding to the underscore implementation. */

#include <stdint.h>

extern void _rasterizer_decals_begin(int16_t layer);

void rasterizer_decals_begin(int16_t layer)
{
    _rasterizer_decals_begin(layer);
}
