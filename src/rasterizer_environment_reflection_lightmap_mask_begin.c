/* rasterizer_environment_reflection_lightmap_mask_begin @0x836A6280 */
/* rasterizer_environment_reflection_lightmap_mask_begin 0x836A6280 (thunk) — tail-call thunk to the underscore rasterizer implementation. */
struct bitmap_data;

#include "headers/bitmap_data.h"
extern void _rasterizer_environment_reflection_lightmap_mask_begin(const bitmap_data *lightmap);

void rasterizer_environment_reflection_lightmap_mask_begin(const struct bitmap_data *lightmap)
{
    _rasterizer_environment_reflection_lightmap_mask_begin(lightmap);
}
