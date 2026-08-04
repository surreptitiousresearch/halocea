/* rasterizer_window_set_fog @0x836A5D90 — thunk to the platform implementation
 * _rasterizer_window_set_fog. */

#include "headers/render_fog.h"

extern void _rasterizer_window_set_fog(const render_fog *fog);

void rasterizer_window_set_fog(const render_fog *fog)
{
    _rasterizer_window_set_fog(fog);
}
