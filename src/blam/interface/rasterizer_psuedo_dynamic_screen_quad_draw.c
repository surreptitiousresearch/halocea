/* rasterizer_psuedo_dynamic_screen_quad_draw @0x836A63A0 — linker thunk: an unconditional tail-branch to
 * the real implementation _rasterizer_psuedo_dynamic_screen_quad_draw. Present so callers in this module
 * can reference the unprefixed name. */

#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/dynamic_screen_vertex.h"

extern void _rasterizer_psuedo_dynamic_screen_quad_draw(const rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *vertices);

void rasterizer_psuedo_dynamic_screen_quad_draw(rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *vertices)
{
    _rasterizer_psuedo_dynamic_screen_quad_draw(parameters, vertices);
}
