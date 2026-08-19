/* rasterizer_window_begin @0x836A5D80 — thunk to the platform rasterizer window-begin implementation. */

#include "headers/rasterizer_window_begin_parameters.h"

extern void _rasterizer_window_begin(const rasterizer_window_begin_parameters *parameters);

void rasterizer_window_begin(const rasterizer_window_begin_parameters *parameters)
{
    _rasterizer_window_begin(parameters);
}
