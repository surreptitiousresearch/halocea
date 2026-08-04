/* rasterizer_decals_draw @0x836A60E0 — tail-call thunk forwarding to the underscore implementation. */

#include <stdint.h>

extern void _rasterizer_decals_draw(int16_t cluster_index);

void rasterizer_decals_draw(int16_t cluster_index)
{
    _rasterizer_decals_draw(cluster_index);
}
