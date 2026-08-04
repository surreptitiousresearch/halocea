#include <string.h>
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/render_fog.h"

void _rasterizer_window_get_fog(render_fog *fog)
{
    memcpy(fog, &global_window_parameters.fog, sizeof(render_fog));
}
