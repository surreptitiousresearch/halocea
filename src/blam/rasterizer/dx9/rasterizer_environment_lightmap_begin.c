/* rasterizer_environment_lightmap_begin @0x836A6188 */
/* rasterizer_environment_lightmap_begin 0x836A6188 (thunk) — tail-call thunk to the underscore rasterizer implementation. */
struct bitmap_data;

#include "headers/bitmap_data.h"
extern void _rasterizer_environment_lightmap_begin(const bitmap_data *lightmap);

void rasterizer_environment_lightmap_begin(const struct bitmap_data *lightmap)
{
    _rasterizer_environment_lightmap_begin(lightmap);
}
