/* _rasterizer_widget_draw_sprite3d @0x83786D48 — draw a world-space billboard sprite for the UI/widget layer.
 * The 3D `point` (with world `radius`) is projected to a screen-space center, depth and axis via
 * rasterizer_widget_project_billboard; if the point is behind the camera or the radius is non-positive nothing
 * is drawn. The projected axis is rotated by `rotation` (degrees) and scaled per-axis by `scale` (defaulting to
 * 1,1 when null), then six vertices (two triangles) forming the quad are appended to the current widget batch,
 * each tinted by `color` and the global widget specular colour. The batch is flushed when it fills.
 *
 * DEVIATION: the FPR-shadow ABI scrambled the decompiler's view (radius/rotation floats burn GPR slots) — it
 * mislabeled the real `scale` pointer as `color` and the real uint `color` as a phantom trailing arg. The
 * DB/PDB 5-arg prototype is authoritative; disasm (mr r30,r5 = scale; mr r31,r7 = color; f31=f2=rotation)
 * settles the mapping. Register-pun doubles are plain float math; the interleaved per-vertex stores are
 * flattened into vertex order. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector2d.h"
#include "headers/widget_batch.h"
#include <math.h>
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"


extern uint8_t rasterizer_widget_project_billboard(const real_point3d *point, float radius, real_point3d *projected_center, real_vector2d *projected_axes);
extern int find_batch(void);
extern void flush_batch(int batch_index);

void _rasterizer_widget_draw_sprite3d(const real_point3d *point, float radius, const real_vector2d *scale,
        float rotation, unsigned int color)
{
    real_point3d projected_center;
    real_vector2d projected_axes;
    if ( !(radius > 0.0f)
      || !rasterizer_widget_project_billboard(point, radius, &projected_center, &projected_axes) )
        return;

    float axis_x;   /* rotated axis feeding the horizontal corner offsets */
    float axis_y;   /* rotated axis feeding the vertical corner offsets */
    if ( rotation == 0.0f )
    {
        axis_x = projected_axes.n[0];
        axis_y = projected_axes.n[1];
    }
    else
    {
        float radians = rotation * DEG_TO_RAD;
        float cos_r = (float)cos(radians);
        float sin_r = (float)sin(radians);
        axis_y = projected_axes.n[0] * sin_r + projected_axes.n[1] * cos_r;
        axis_x = projected_axes.n[0] * cos_r - projected_axes.n[1] * sin_r;
    }

    float scale_x = scale ? scale->n[0] : 1.0f;
    float scale_y = scale ? scale->n[1] : 1.0f;

    float cx = projected_center.n[0];
    float cy = projected_center.n[1];
    float z  = projected_center.n[2];

    float x_left  = cx - scale_x * axis_x;
    float x_right = cx + scale_x * axis_x;
    float x_topd  = cx + scale_x * axis_y;
    float x_botd  = cx - scale_x * axis_y;
    float y_top   = cy - scale_y * axis_y;
    float y_bot   = cy + scale_y * axis_y;
    float y_topd  = cy - scale_y * axis_x;
    float y_botd  = cy + scale_y * axis_x;

    int batch = find_batch();
    widget_batch *b = &widget_batches[batch];
    unsigned int vertex_index = b->vertex_index;
    dynamic_unlit_vertex_spec *v = &b->vertex_data[vertex_index];
    unsigned int specular = widget_specular_color;

    v[0].position.n[0] = x_left;  v[0].position.n[1] = y_top;  v[0].position.n[2] = z;
    v[0].color = color; v[0].specular = specular; v[0].texcoord.n[0] = 0.0f; v[0].texcoord.n[1] = 0.0f;
    v[1].position.n[0] = x_topd;  v[1].position.n[1] = y_topd; v[1].position.n[2] = z;
    v[1].color = color; v[1].specular = specular; v[1].texcoord.n[0] = 1.0f; v[1].texcoord.n[1] = 0.0f;
    v[2].position.n[0] = x_right; v[2].position.n[1] = y_bot;  v[2].position.n[2] = z;
    v[2].color = color; v[2].specular = specular; v[2].texcoord.n[0] = 1.0f; v[2].texcoord.n[1] = 1.0f;
    v[3].position.n[0] = x_left;  v[3].position.n[1] = y_top;  v[3].position.n[2] = z;
    v[3].color = color; v[3].specular = specular; v[3].texcoord.n[0] = 0.0f; v[3].texcoord.n[1] = 0.0f;
    v[4].position.n[0] = x_right; v[4].position.n[1] = y_bot;  v[4].position.n[2] = z;
    v[4].color = color; v[4].specular = specular; v[4].texcoord.n[0] = 1.0f; v[4].texcoord.n[1] = 1.0f;
    v[5].position.n[0] = x_botd;  v[5].position.n[1] = y_botd; v[5].position.n[2] = z;
    v[5].color = color; v[5].specular = specular; v[5].texcoord.n[0] = 0.0f; v[5].texcoord.n[1] = 1.0f;

    b->vertex_index = vertex_index + 6;
    if ( vertex_index == 3066 )
        flush_batch(batch);
}
