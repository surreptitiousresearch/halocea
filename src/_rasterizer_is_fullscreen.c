#include "headers/rasterizer_engine_globals.h"

int _rasterizer_is_fullscreen(void)
{
    if (!use_fullscreen)
        return 0;
    if (!global_d3d_device)
        return 0;
    return 1;
}
