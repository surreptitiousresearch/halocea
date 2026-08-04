#include "headers/render_fog.h"

extern void _rasterizer_window_get_fog(render_fog *fog);

void rasterizer_window_get_fog(render_fog *fog)
{
    _rasterizer_window_get_fog(fog);
}
