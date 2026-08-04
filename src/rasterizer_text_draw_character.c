/* rasterizer_text_draw_character @0x83814C88 — submit one glyph quad (4 vertices, 24-byte stride) as a
 * triangle fan through the user-pointer draw path. Only draws when dynamic screen geometry is enabled and
 * the target is the main framebuffer. */

#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/d3d_render_boundary.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"
#include "headers/_D3DPRIMITIVETYPE.h"


extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
                                     const void *vertex_data, unsigned int vertex_stride);

void rasterizer_text_draw_character(const dynamic_screen_vertex *vertices)
{
    if (rasterizer_debug_options.draw_dynamic_screen_geometry && global_window_parameters.rasterizer_target == _rasterizer_target_render_primary)
        D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLESTRIP /* deviation: old comment said FAN; DB _D3DPRIMITIVETYPE: 6 = TRIANGLESTRIP */, 4, vertices, 0x18);
}
