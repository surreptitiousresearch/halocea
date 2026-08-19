/* render_camera_build_frustum @0x8376BE60 — build a render_frustum from a camera and (optional) sub-window
 * frustum bounds: the view<->world matrices, the six world-space clip planes (left/right/bottom/top/near/
 * far), the five world-space frustum corner vertices + apex, the world-space AABB, the frustum midpoint,
 * and (when requested) the projection matrix and world-to-screen scale.
 *
 * The Hex-Rays output for this function is flagged "local variable allocation has failed" — the body was
 * reconstructed against the disassembly. The reg-allocation failure produced three artifacts, all fixed
 * here:
 *   1. A phantom 5th parameter `long double a5`. It is not a parameter (DB prototype has 4 args); it is a
 *      local scratch double holding half the vertical FOV. Modeled as `half_vertical_fov`.
 *   2. The viewport width/height, computed from the integer viewport rectangle, were rendered as a garbage
 *      `*(__int64 *)(&v5 - 1)` load (v5 = r2, OVERLAPPED). Disasm (0x8376BE60-0x8376BEE0) shows
 *      viewport_width = x1 - x0 and viewport_height = y1 - y0, converted to float.
 *   3. The decompiler packed the tangent and the aspect term into one `long double v17` (two double slots);
 *      split here into `tan_half_fov` and `aspect_term`.
 *
 * The world-AABB expansion loop the decompiler renders as reinterpreted pointer arithmetic into
 * frustum->frustum_bounds (e.g. `v87[56]`, `frustum_bounds.n[3*(i+19)]`) is exactly an iteration over
 * frustum->world_vertices[1..4]; reproduced as such.
 */

#include <stdint.h>
#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/ppc_intrinsics.h"
#include <math.h>
#include <string.h>
#include "headers/blam_data_globals.h"


extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern real_plane3d *matrix4x3_transform_plane(const real_matrix4x3 *matrix, const real_plane3d *plane, real_plane3d *result);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);

void render_camera_build_frustum(const render_camera *camera, const real_rectangle2d *frustum_bounds,
    render_frustum *frustum, uint8_t build_projection)
{
    float viewport_width = (float)(camera->viewport_bounds.__s1.x1 - camera->viewport_bounds.__s1.x0);
    float viewport_height = (float)(camera->viewport_bounds.__s1.y1 - camera->viewport_bounds.__s1.y0);

    if ( frustum_bounds )
        frustum->frustum_bounds = *frustum_bounds;
    else
    {
        frustum->frustum_bounds.n[2] = -1.0f;
        frustum->frustum_bounds.n[0] = -1.0f;
        frustum->frustum_bounds.n[3] = 1.0f;
        frustum->frustum_bounds.n[1] = 1.0f;
    }

    double half_vertical_fov = (camera->vertical_field_of_view * 0.5f);

    float half_bounds_width  = (frustum->frustum_bounds.n[1] - frustum->frustum_bounds.n[0]) * 0.5f;
    float half_bounds_height = (frustum->frustum_bounds.n[3] - frustum->frustum_bounds.n[2]) * 0.5f;
    float bounds_center_x = ((frustum->frustum_bounds.n[0] + frustum->frustum_bounds.n[1]) / half_bounds_width) * -0.5f;
    float bounds_center_y = ((frustum->frustum_bounds.n[2] + frustum->frustum_bounds.n[3]) / half_bounds_height) * -0.5f;

    double tan_half_fov = tan(half_vertical_fov);
    double aspect_term = (half_bounds_width / viewport_height);

    /* Build the view->world basis: row 0 = normalize(forward x up), row 1 = normalize(that x forward),
     * row 2 = normalize(-forward), row 3 = camera position. */
    double up_x = camera->up.n[0];
    double forward_z = camera->forward.n[2];
    double forward_x = camera->forward.n[0];
    double forward_x_up_z = (camera->forward.n[0] * camera->up.n[2]);
    double forward_y = camera->forward.n[1];
    double neg_forward_y = -forward_y;

    double half_bounds_height_scaled = ((float)tan_half_fov * half_bounds_height);

    double left_z = ((camera->forward.n[0] * camera->up.n[1])
                          - (camera->forward.n[1] * camera->up.n[0]));
    double neg_forward_z = -forward_z;
    double left_x = ((camera->forward.n[1] * camera->up.n[2])
                          - (camera->forward.n[2] * camera->up.n[1]));
    double neg_forward_x = -forward_x;
    double left_y = (((float)forward_z * (float)up_x) - (float)forward_x_up_z);

    double x_scale = (1.0f / (((float)aspect_term * viewport_width) * (float)tan_half_fov)); /* projection X scale (v32) */
    double forward_x_left_y = ((float)forward_x * (float)left_y);
    double y_scale = (1.0f / (float)half_bounds_height_scaled); /* projection Y scale (v30) */

    double up2_x = (((float)forward_z * (float)left_y) - ((float)forward_y * (float)left_z));
    double up2_y = (((float)forward_x * (float)left_z) - ((float)forward_z * (float)left_x));
    double up2_z = ((float)forward_y * (float)left_x) - (float)forward_x_left_y;

    float basis_left[3]  = { (float)left_x, (float)left_y, (float)left_z };
    float basis_up[3]    = { (float)up2_x, (float)up2_y, (float)up2_z };
    float basis_back[3]  = { (float)neg_forward_x, (float)neg_forward_y, (float)neg_forward_z };

    double left_len = __fsqrts((((float)left_y * (float)left_y)
                             + (((float)left_x * (float)left_x) + ((float)left_z * (float)left_z))));
    if ( __fabs(left_len) >= 0.000099999997f )
    {
        double inv = (1.0f / (float)left_len);
        basis_left[0] = (float)inv * (float)left_x;
        basis_left[1] = (float)left_y * (float)inv;
        basis_left[2] = (float)left_z * (float)inv;
    }

    double up_len = __fsqrts((((float)up2_y * (float)up2_y)
                           + (((float)up2_x * (float)up2_x) + ((float)up2_z * (float)up2_z))));
    if ( __fabs(up_len) >= 0.000099999997f )
    {
        double inv = (1.0f / (float)up_len);
        basis_up[0] = (float)inv * (float)up2_x;
        basis_up[1] = (float)up2_y * (float)inv;
        basis_up[2] = (((float)forward_y * (float)left_x) - (float)forward_x_left_y) * (float)inv;
    }

    double back_len = __fsqrts((((float)neg_forward_y * (float)neg_forward_y)
                             + (((float)neg_forward_x * (float)neg_forward_x)
                                     + ((float)neg_forward_z * (float)neg_forward_z))));
    if ( __fabs(back_len) >= 0.000099999997f )
    {
        double inv = (1.0f / (float)back_len);
        basis_back[0] = (float)inv * (float)neg_forward_x;
        basis_back[1] = (float)neg_forward_y * (float)inv;
        basis_back[2] = (float)neg_forward_z * (float)inv;
    }

    frustum->view_to_world.___u1.n[0][0] = basis_left[0];
    frustum->view_to_world.___u1.n[0][2] = basis_left[2];
    frustum->view_to_world.___u1.n[2][2] = basis_back[2];
    frustum->view_to_world.___u1.n[1][2] = basis_up[2];
    frustum->view_to_world.___u1.n[2][0] = basis_back[0];
    frustum->view_to_world.___u1.n[2][1] = basis_back[1];
    frustum->view_to_world.___u1.n[0][1] = basis_left[1];
    frustum->view_to_world.___u1.n[1][0] = basis_up[0];
    frustum->view_to_world.___u1.n[1][1] = basis_up[1];
    frustum->view_to_world.___u1.n[3][0] = camera->position.n[0];
    frustum->view_to_world.___u1.n[3][1] = camera->position.n[1];
    frustum->view_to_world.scale = 1.0f;
    frustum->view_to_world.___u1.n[3][2] = camera->position.n[2];
    matrix4x3_inverse(&frustum->view_to_world, &frustum->world_to_view);

    real_plane3d view_plane;
    real_vector3d plane_normal;

    /* Left plane. */
    plane_normal.n[0] = -(float)x_scale;
    plane_normal.n[2] = bounds_center_x + 1.0f;
    plane_normal.n[1] = 0.0f;
    {
        float len = __fsqrts(((plane_normal.n[2] * plane_normal.n[2])
                                   + (plane_normal.n[0] * plane_normal.n[0])));
        if ( __fabs(len) >= 0.000099999997f )
        {
            float inv = 1.0f / len;
            plane_normal.n[0] *= inv;
            plane_normal.n[1] *= inv;
            plane_normal.n[2] *= inv;
        }
        view_plane.n = plane_normal;
        view_plane.d = (plane_normal.n[0] * global_origin3d->n[0])
            + ((global_origin3d->n[2] * plane_normal.n[2])
                    + (global_origin3d->n[1] * plane_normal.n[1]));
        matrix4x3_transform_plane(&frustum->view_to_world, &view_plane, &frustum->world_planes[0]);
    }

    /* Right plane. */
    plane_normal.n[2] = 1.0f - bounds_center_x;
    plane_normal.n[0] = (float)x_scale;
    plane_normal.n[1] = 0.0f;
    {
        float len = __fsqrts(((plane_normal.n[2] * plane_normal.n[2])
                                   + ((float)x_scale * (float)x_scale)));
        if ( __fabs(len) >= 0.000099999997f )
        {
            float inv = 1.0f / len;
            plane_normal.n[0] *= inv;
            plane_normal.n[1] *= inv;
            plane_normal.n[2] *= inv;
        }
        view_plane.n = plane_normal;
        view_plane.d = (plane_normal.n[0] * global_origin3d->n[0])
            + ((global_origin3d->n[2] * plane_normal.n[2])
                    + (global_origin3d->n[1] * plane_normal.n[1]));
        matrix4x3_transform_plane(&frustum->view_to_world, &view_plane, &frustum->world_planes[1]);
    }

    /* Bottom plane. */
    plane_normal.n[1] = -(float)y_scale;
    plane_normal.n[0] = 0.0f;
    plane_normal.n[2] = bounds_center_y + 1.0f;
    {
        float len = __fsqrts(((plane_normal.n[2] * plane_normal.n[2])
                                   + (plane_normal.n[1] * plane_normal.n[1])));
        if ( __fabs(len) >= 0.000099999997f )
        {
            float inv = 1.0f / len;
            plane_normal.n[0] *= inv;
            plane_normal.n[1] *= inv;
            plane_normal.n[2] *= inv;
        }
        view_plane.n = plane_normal;
        view_plane.d = (plane_normal.n[0] * global_origin3d->n[0])
            + ((global_origin3d->n[2] * plane_normal.n[2])
                    + (global_origin3d->n[1] * plane_normal.n[1]));
        matrix4x3_transform_plane(&frustum->view_to_world, &view_plane, &frustum->world_planes[2]);
    }

    /* Top plane. */
    plane_normal.n[2] = 1.0f - bounds_center_y;
    plane_normal.n[0] = 0.0f;
    plane_normal.n[1] = 1.0f / (float)half_bounds_height_scaled;
    {
        float len = __fsqrts(((plane_normal.n[2] * plane_normal.n[2])
                                   + (plane_normal.n[1] * plane_normal.n[1])));
        if ( __fabs(len) >= 0.000099999997f )
        {
            float inv = 1.0f / len;
            plane_normal.n[0] *= inv;
            plane_normal.n[1] = inv * (1.0f / (float)half_bounds_height_scaled);
            plane_normal.n[2] *= inv;
        }
        view_plane.n = plane_normal;
        view_plane.d = (plane_normal.n[0] * global_origin3d->n[0])
            + ((global_origin3d->n[2] * plane_normal.n[2])
                    + (global_origin3d->n[1] * plane_normal.n[1]));
        matrix4x3_transform_plane(&frustum->view_to_world, &view_plane, &frustum->world_planes[3]);
    }

    /* Near plane. */
    view_plane.n.n[0] = 0.0f;
    view_plane.n.n[1] = 0.0f;
    view_plane.n.n[2] = 1.0f;
    view_plane.d = -camera->z_near;
    matrix4x3_transform_plane(&frustum->view_to_world, &view_plane, &frustum->world_planes[4]);

    /* Far plane. */
    view_plane.n.n[0] = 0.0f;
    view_plane.n.n[1] = 0.0f;
    view_plane.n.n[2] = -1.0f;
    view_plane.d = camera->z_far;
    matrix4x3_transform_plane(&frustum->view_to_world, &view_plane, &frustum->world_planes[5]);

    frustum->z_near = camera->z_near;
    frustum->z_far = camera->z_far;

    /* Far-plane corner vertices (+ apex) and frustum midpoint. */
    double far_extent_y = ((1.0f / (1.0f / (float)half_bounds_height_scaled)) * camera->z_far);
    double far_extent_x = -((1.0f / (float)x_scale) * camera->z_far);
    double z_span = (camera->z_far + camera->z_near);

    real_point3d corner;
    corner.n[2] = -camera->z_far;
    double corner_x_pos = (bounds_center_x + 1.0f) * (float)far_extent_x;
    corner.n[0] = (float)corner_x_pos;
    double corner_x_neg = (bounds_center_x - 1.0f) * (float)far_extent_x;
    double half_z_span = ((float)z_span * 0.5f);
    double corner_y_pos = (bounds_center_y + 1.0f) * -(float)far_extent_y;
    corner.n[1] = (float)corner_y_pos;
    double corner_y_neg = (bounds_center_y - 1.0f) * -(float)far_extent_y;
    matrix4x3_transform_point(&frustum->view_to_world, &corner, &frustum->world_vertices[0]);

    corner.n[0] = (float)corner_x_neg;
    corner.n[1] = (float)corner_y_pos;
    corner.n[2] = -camera->z_far;
    matrix4x3_transform_point(&frustum->view_to_world, &corner, &frustum->world_vertices[1]);

    corner.n[0] = (float)corner_x_pos;
    corner.n[1] = (float)corner_y_neg;
    corner.n[2] = -camera->z_far;
    matrix4x3_transform_point(&frustum->view_to_world, &corner, &frustum->world_vertices[2]);

    corner.n[0] = (float)corner_x_neg;
    corner.n[1] = (float)corner_y_neg;
    corner.n[2] = -camera->z_far;
    matrix4x3_transform_point(&frustum->view_to_world, &corner, &frustum->world_vertices[3]);

    frustum->world_vertices[4].n[0] = camera->position.n[0];
    real_point3d midpoint;
    midpoint.n[2] = -(float)half_z_span;
    midpoint.n[0] = -(((1.0f / (float)x_scale) * (float)half_z_span) * bounds_center_x);
    midpoint.n[1] = -(((1.0f / (float)y_scale) * (float)half_z_span) * bounds_center_y);
    frustum->world_vertices[4].n[1] = camera->position.n[1];
    frustum->world_vertices[4].n[2] = camera->position.n[2];
    matrix4x3_transform_point(&frustum->view_to_world, &midpoint, &frustum->world_midpoint);

    /* World-space AABB over the five frustum vertices. */
    frustum->world_bounds.n[1] = frustum->world_vertices[0].n[0];
    frustum->world_bounds.n[0] = frustum->world_vertices[0].n[0];
    frustum->world_bounds.n[3] = frustum->world_vertices[0].n[1];
    frustum->world_bounds.n[2] = frustum->world_vertices[0].n[1];
    frustum->world_bounds.n[5] = frustum->world_vertices[0].n[2];
    frustum->world_bounds.n[4] = frustum->world_vertices[0].n[2];
    for ( int i = 1; i < 5; i++ )
    {
        float vx = frustum->world_vertices[i].n[0];
        float vy = frustum->world_vertices[i].n[1];
        float vz = frustum->world_vertices[i].n[2];
        if ( frustum->world_bounds.n[0] > vx ) frustum->world_bounds.n[0] = vx;
        if ( frustum->world_bounds.n[2] > vy ) frustum->world_bounds.n[2] = vy;
        if ( frustum->world_bounds.n[4] > vz ) frustum->world_bounds.n[4] = vz;
        if ( frustum->world_bounds.n[1] <= vx ) frustum->world_bounds.n[1] = vx;
        if ( frustum->world_bounds.n[3] <= vy ) frustum->world_bounds.n[3] = vy;
        if ( frustum->world_bounds.n[5] <= vz ) frustum->world_bounds.n[5] = vz;
    }

    if ( build_projection )
    {
        /* Oblique near-clip against the mirror plane when z_near == 0, otherwise a standard near plane. */
        float mirror_nx, mirror_ny, mirror_nz, mirror_d;
        if ( camera->z_near == 0.0f )
        {
            real_plane3d mirror_view;
            matrix4x3_transform_plane(&frustum->world_to_view, &camera->mirror_plane, &mirror_view);
            mirror_d = mirror_view.d;
            mirror_nz = mirror_view.n.n[2];
            mirror_ny = mirror_view.n.n[1];
            mirror_nx = mirror_view.n.n[0];
        }
        else
        {
            mirror_nx = 0.0f;
            mirror_ny = 0.0f;
            mirror_nz = 1.0f;
            mirror_d = -camera->z_near;
        }

        double clip_offset = -((1.0f / mirror_nz) * mirror_d);
        double q = camera->z_far
            / ((__fabs(((1.0f / mirror_nz) * mirror_ny))
              + __fabs(((1.0f / mirror_nz) * mirror_nx))
              + 1.0)
             * (camera->z_far - (float)clip_offset));
        double proj_zz = (float)q;
        double proj_xz = (((1.0f / mirror_nz) * (float)q) * mirror_nx);
        double proj_yz = (((1.0f / mirror_nz) * (float)q) * mirror_ny);
        double proj_wz = -((float)q * (float)clip_offset);
        if ( proj_wz > 0.0 && camera->z_near == 0.0f )
        {
            proj_xz = -proj_xz;
            proj_yz = -proj_yz;
            proj_zz = -(float)q;
            proj_wz = -proj_wz;
        }

        memset(frustum->projection_matrix, 0, sizeof(frustum->projection_matrix));
        frustum->projection_matrix[0][0] = (float)x_scale;
        frustum->projection_matrix[0][2] = -(float)proj_xz;
        frustum->projection_matrix[1][1] = (float)y_scale;
        frustum->projection_matrix[1][2] = -(float)proj_yz;
        frustum->projection_matrix[2][0] = -bounds_center_x;
        frustum->projection_matrix[2][1] = -bounds_center_y;
        frustum->projection_valid = 1;
        frustum->projection_matrix[2][2] = -(float)proj_zz;
        frustum->projection_matrix[2][3] = -1.0f;
        frustum->projection_matrix[3][2] = (float)proj_wz;
        frustum->projection_world_to_screen.n[0] = ((float)x_scale * viewport_width) * 0.5f;
        frustum->projection_world_to_screen.n[1] = ((float)y_scale * viewport_height) * 0.5f;
    }
    else
    {
        frustum->projection_valid = 0;
    }
}
