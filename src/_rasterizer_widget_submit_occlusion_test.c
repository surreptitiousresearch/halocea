/* _rasterizer_widget_submit_occlusion_test 0x83786170 — issue a hardware occlusion query for a lens-flare
 * billboard. Projects the world-space billboard (point + radius) to a screen rectangle, computes the expected
 * pixel coverage (two-thirds of the rectangle area, clamped non-negative), and — when the hardware supports
 * occlusion queries — wraps a textured quad covering that rectangle in a begin/end occlusion query. Returns the
 * expected coverage in pixels (1 when occlusion is disabled, 4 when unsupported, 0 when off-screen or out of
 * query slots). The actual visible-pixel count is read back later from occlusion_query[index]. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector2d.h"
#include "headers/widget_occlusion_vertex.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_occlusion_globals.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


extern uint8_t rasterizer_widget_project_billboard(const real_point3d *point, float radius, real_point3d *projected_center, real_vector2d *projected_axes);
extern void D3DQuery_Issue(D3DQuery *query, unsigned int flags);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
                                     const void *vertex_data, unsigned int vertex_stride);
extern double floor(double x);

/* DEVIATION: the database prototype lists 3 args (point, radius, int index); the decompiler grew a phantom
 * real_point3d* parameter out of the projected_center stack buffer and split the index into a 4th 'a4'. r5 (the
 * occlusion-query slot index, bounds-checked < 1024) is the real third argument. */
int _rasterizer_widget_submit_occlusion_test(const real_point3d *point, float radius, int index)
{
    if ( !rasterizer_debug_options.lens_flare_occlusion_enabled )
        return 1;

    real_point3d projected_center;
    real_vector2d projected_axes;
    if ( index >= 1024 || !rasterizer_widget_project_billboard(point, radius, &projected_center, &projected_axes) )
        return 0;

    /* clamp the projected half-extents to at least one pixel */
    float half_width = projected_axes.n[0];
    if ( half_width < 1.0f )
        half_width = 1.0f;
    float half_height = projected_axes.n[1];
    if ( half_height < 1.0f )
        half_height = 1.0f;

    /* screen-rectangle corners: each clamped to the signed-16-bit range and floored to whole pixels */
    float left_f = (projected_center.n[0] - half_width);
    if ( left_f < -32767.0f )
        left_f = -32767.0f;
    else if ( left_f > 32767.0f )
        left_f = 32767.0f;
    int16_t left = (int)(float)floor(left_f);

    float top_f = (projected_center.n[1] - half_height);
    if ( top_f < -32767.0f )
        top_f = -32767.0f;
    else if ( top_f > 32767.0f )
        top_f = 32767.0f;
    int16_t top = (int)(float)floor(top_f);

    float right_f = (projected_center.n[0] + half_width);
    if ( right_f < -32767.0f )
        right_f = -32767.0f;
    else if ( right_f > 32767.0f )
        right_f = 32767.0f;
    int16_t right = (int)(float)floor(right_f);

    float bottom_f = (projected_center.n[1] + half_height);
    if ( bottom_f < -32767.0f )
        bottom_f = -32767.0f;
    else if ( bottom_f > 32767.0f )
        bottom_f = 32767.0f;
    int16_t bottom = (int16_t)(int)(float)floor(bottom_f);

    int rectangle_area = (right - left) * (bottom - top);
    /* two-thirds of the (non-negative) rectangle area = expected coverage in pixels */
    int expected_coverage = 2 * (rectangle_area < 0 ? 0 : rectangle_area) / 3;

    if ( !supports_occlusion_test )
        return 4;

    if ( expected_coverage > 0 )
    {
        D3DQuery *query = occlusion_query[index];
        if ( query )
        {
            D3DQuery_Issue(query, 2u);  /* D3DISSUE_BEGIN */

            float left_x   = (float)left;
            float top_y    = (float)top;
            float right_x  = (float)right;
            float bottom_y = (float)bottom;
            float depth    = projected_center.n[2];

            widget_occlusion_vertex quad[4];
            quad[0].position.n[0] = left_x;  quad[0].position.n[1] = top_y;    quad[0].position.n[2] = depth;
            quad[0].diffuse = 0xFFFFFFFF;    quad[0].specular = 0xFFFFFFFF;
            quad[0].texcoord.n[0] = 0.0f;    quad[0].texcoord.n[1] = 0.0f;

            quad[1].position.n[0] = right_x; quad[1].position.n[1] = top_y;    quad[1].position.n[2] = depth;
            quad[1].diffuse = 0xFFFFFFFF;    quad[1].specular = 0xFFFFFFFF;
            quad[1].texcoord.n[0] = 1.0f;    quad[1].texcoord.n[1] = 0.0f;

            quad[2].position.n[0] = right_x; quad[2].position.n[1] = bottom_y; quad[2].position.n[2] = depth;
            quad[2].diffuse = 0xFFFFFFFF;    quad[2].specular = 0xFFFFFFFF;
            quad[2].texcoord.n[0] = 1.0f;    quad[2].texcoord.n[1] = 1.0f;

            quad[3].position.n[0] = left_x;  quad[3].position.n[1] = bottom_y; quad[3].position.n[2] = depth;
            quad[3].diffuse = 0xFFFFFFFF;    quad[3].specular = 0xFFFFFFFF;
            quad[3].texcoord.n[0] = 0.0f;    quad[3].texcoord.n[1] = 1.0f;

            D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLEFAN, 4u, quad, 0x1Cu);
            D3DQuery_Issue(occlusion_query[index], 1u);  /* D3DISSUE_END */
        }
    }
    return expected_coverage;
}
