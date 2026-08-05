/* build_sprite @0x837EC4E8 — append one sprite quad (4 corners, 24-byte dynamic vertices) to a
 * build_sprite_data batch. Looks the sprite frame up in the bitmap group's sequence/sprite-sheet blocks,
 * rotates the corner offsets about the sprite's registration point by `rotation`, places them either
 * directly in screen space (data->flags bit 0) or along the compute_basis billboard rows scaled by
 * bitmap-width * scale, packs the ARGB color (alpha either replaced by or modulated with `fade` depending
 * on the shader's framebuffer blend), and accumulates the batch centroid. World-space sprites also
 * accumulate a view-space bounding box: sprites covering > 0.5 of the view count toward a big-sprite
 * budget, and once more than 10 are queued the just-written quad is dropped again (count rollback).
 * `debug_sprites` draws the quad's wire outline via view_to_world.
 *
 * DEVIATION: the decompiler fabricated a 35-parameter signature (FPR-shadow phantom pile: the three
 * float args rotation/scale/fade burn GPR shadow slots, spilling `color` and `flags` to the stack).
 * DB prototype is 11 args; the decompiler's `a31` = `color`, `a35` = `flags` (confirmed: a31->n[i]*255
 * are the ARGB channels, a35 bits 2/4 are the mirror flags, and disasm 0x837EC680 `lwz r29, arg_64` →
 * `mr r4, r29` passes it as transform_origin_and_direction's flags). Its own named `color`/`flags`
 * params (9th/11th) are phantoms and never used.
 * DEVIATION: the build_sprite_compute_basis call is 5-arg per its committed reconstruction — disasm
 * 0x837EC68C-A0 shows r5=&transformed_origin, r6=&transformed_direction, r7=&basis; the decompiler's
 * trailing r8 matrix argument is a phantom.
 * DEVIATION: `global_null_rectangle3d` is a POINTER to the template rectangle (private_null_rectangle):
 * disasm 0x837EC61C `lwz r11, global_null_rectangle3d@l` loads the pointer, then the 6-dword copy loop
 * reads through it. The decompiler rendered it as an array base (`global_null_rectangle3d[-1].n[5]`). */

#include <stdint.h>
#include "headers/build_sprite_data.h"
#include "headers/shader_effect_flags.h"
#include "headers/build_sprite_flags.h"
#include "headers/build_sprite_group.h"
#include "headers/build_sprite_globals.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_group_sprite.h"
#include "headers/bitmap_data.h"
#include "headers/shader_effect.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_argb_color.h"
#include "headers/real_rectangle3d.h"
#include "headers/global_tag_instances.h"
#include "headers/render_globals.h"

#include <math.h>
#include "headers/blam_data_globals.h"


extern int16_t build_sprite_get_group(build_sprite_data *data, bitmap_data *bitmap);
extern void build_sprite_transform_origin_and_direction(const build_sprite_data *data, unsigned int flags, const real_point3d *untransformed_origin, const real_vector3d *untransformed_direction, real_point3d *transformed_origin, real_vector3d *transformed_direction);
extern void build_sprite_compute_basis(const build_sprite_data *data, int16_t mode, const real_vector3d *up_reference, const real_point3d *facing_direction, real_vector3d *basis);
extern float build_sprite_compute_vertex_fade(int16_t fade_mode, const real_point3d *viewer_point, const real_vector3d *viewer_normal);
extern float render_frustum_cube_view_fraction(const render_frustum *frustum, const real_rectangle3d *bounds);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void rasterizer_debug_line(const real_point3d *p0, const real_point3d *p1, const real_argb_color *color);

extern void build_sprite_compute_scale(const build_sprite_data *data, int16_t mode, unsigned int flags, const real_point3d *origin, const bitmap_data *bitmap, float *scale);
void build_sprite(build_sprite_data *data, int16_t mode, int16_t sequence_index, int16_t sprite_index, const real_point3d *untransformed_origin, const real_vector3d *untransformed_direction, float rotation, float scale, const real_argb_color *color, float fade, unsigned int flags)
{
    bitmap_group *group = TAG_GET(bitmap_group, data->bitmap_group_index);

    if (!color)
        color = global_real_argb_white;

    if (data->sprite_count < data->maximum_sprite_count
        && sequence_index >= 0 && sequence_index < group->sequences.count)
    {
        bitmap_group_sequence *sequence = (bitmap_group_sequence *)group->sequences.address + sequence_index;
        if (sequence->first_bitmap_index != -1
            && sprite_index >= 0 && sprite_index < sequence->sprites.count)
        {
            bitmap_group_sprite *sprite = (bitmap_group_sprite *)sequence->sprites.address + sprite_index;
            bitmap_data *bitmap = (bitmap_data *)group->bitmaps.address + sprite->bitmap_index;
            int group_index = build_sprite_get_group(data, bitmap);
            if (group_index != -1)
            {
                build_sprite_group *sprite_group = &data->groups[group_index];
                if (sprite_group->sprite_count < data->maximum_sprite_count)
                {
                    int16_t corner_index = 4 * sprite_group->sprite_count;
                    float rotation_sin = 0.0f;
                    float rotation_cos = 1.0f;
                    real_rectangle3d view_bounds = *global_null_rectangle3d;
                    real_point3d transformed_origin;
                    real_vector3d transformed_direction;
                    real_vector3d basis[4]; /* [0] scale slot skipped per compute_basis convention;
                                               rows start at basis[0].j; mode-2 scratch extends to [3] */

                    if (rotation != 0.0f)
                    {
                        rotation_sin = (float)sin(rotation);
                        rotation_cos = (float)cos(rotation);
                    }

                    build_sprite_transform_origin_and_direction(data, flags, untransformed_origin,
                            untransformed_direction, &transformed_origin, &transformed_direction);
                    build_sprite_compute_basis(data, mode, (const real_vector3d *)&transformed_origin,
                            (const real_point3d *)&transformed_direction, basis);

                    /* DEVIATION: inlined copy of build_sprite_compute_scale@0x837EC058 (zero-xref out-of-line twin) collapsed to a call; all 6 args map directly to live host locals (data, mode, flags, &transformed_origin, bitmap, &scale) -- no constant folding, donor's `flags` param is read-through-only (never dereferenced in donor body) but passed for ABI fidelity. */
                    const shader_effect *shader = data->shader;
                    build_sprite_compute_scale(data, mode, flags, &transformed_origin, bitmap, &scale);
                    float sprite_size = scale;
                    /* billboard rows out of the compute_basis skip-scale layout */
                    float forward_x = basis[0].n[1], forward_y = basis[0].n[2], forward_z = basis[1].n[0];
                    float left_x = basis[1].n[1], left_y = basis[1].n[2], left_z = basis[2].n[0];

                    if (shader && shader->effect.framebuffer_fade_mode && mode)
                    {
                        real_vector3d plane_normal; /* forward x left (occupies the mode-2 basis scratch
                                                       slots in the shipped frame layout) */
                        plane_normal.n[1] = (forward_z * left_x) - (left_z * forward_x);
                        plane_normal.n[2] = (left_y * forward_x) - (forward_y * left_x);
                        plane_normal.n[0] = (left_z * forward_y) - (forward_z * left_y);
                        fade = build_sprite_compute_vertex_fade(shader->effect.framebuffer_fade_mode,
                                &transformed_origin, &plane_normal) * fade;
                    }

                    unsigned int packed_color =
                            ((unsigned int)(uint8_t)(int64_t)(color->n[0] * 255.0f) << 24)
                          | ((unsigned int)(uint8_t)(int64_t)(color->n[1] * 255.0f) << 16)
                          | ((unsigned int)(uint8_t)(int64_t)(color->n[2] * 255.0f) << 8)
                          | (uint8_t)(int64_t)(color->n[3] * 255.0f);
                    uint8_t alpha_byte;
                    if (shader && shader->effect.framebuffer_blend_function
                        && (shader->effect.flags & (1u << _shader_effect_uses_nonlinear_tint_bit)) == 0)
                    {
                        alpha_byte = (uint8_t)(int64_t)(fade * 255.0f);
                    }
                    else
                    {
                        alpha_byte = (uint8_t)(int64_t)((float)(packed_color >> 24) * fade);
                    }
                    unsigned int vertex_color = ((unsigned int)alpha_byte << 24) | (packed_color & 0xFFFFFF);

                    for (int16_t corner = 0; ; )
                    {
                        float corner_u = (((corner >> 1) ^ corner) & 1) ? sprite->bounds.__s1.x1
                                                                        : sprite->bounds.__s1.x0;
                        float corner_v = (corner & 2) != 0 ? sprite->bounds.__s1.y0 : sprite->bounds.__s1.y1;

                        float du = corner_u - (sprite->registration_point.n[0] + sprite->bounds.__s1.x0);
                        float dv = (sprite->registration_point.n[1] + sprite->bounds.__s1.y0) - corner_v;
                        float rotated_y = (dv * rotation_cos) + (du * rotation_sin);
                        float rotated_x = (du * rotation_cos) - (dv * rotation_sin);
                        if ((flags & (1u << _build_sprite_u_mirror_bit)) != 0)
                            rotated_x = -rotated_x;
                        if ((flags & (1u << _build_sprite_v_mirror_bit)) != 0)
                            rotated_y = -rotated_y;

                        /* recovered: (char *)vertices + 24*corner_index -> vertices[corner_index] */
                        dynamic_screen_vertex *vertex =
                                (dynamic_screen_vertex *)sprite_group->vertices + corner_index;
                        if ((data->flags & (1u << _build_sprite_viewer_space_bit)) != 0)
                        {
                            vertex->position.n[0] = (rotated_x * sprite_size) + transformed_origin.n[0];
                            vertex->position.n[1] = (rotated_y * sprite_size) + transformed_origin.n[1];
                            /* z intentionally left unwritten in the screen-space path (shipped behavior) */
                        }
                        else
                        {
                            float x = (((left_x * rotated_y)
                                    + (forward_x * rotated_x)) * sprite_size)
                                    + transformed_origin.n[0];
                            float y = (((left_y * rotated_y)
                                    + (forward_y * rotated_x)) * sprite_size)
                                    + transformed_origin.n[1];
                            float z = (((left_z * rotated_y)
                                    + (forward_z * rotated_x)) * sprite_size)
                                    + transformed_origin.n[2];
                            vertex->position.n[0] = x;
                            vertex->position.n[1] = y;
                            vertex->position.n[2] = z;
                            if (x < view_bounds.__s1.x0)
                                view_bounds.__s1.x0 = x;
                            if (x > view_bounds.__s1.x1)
                                view_bounds.__s1.x1 = x;
                            if (y < view_bounds.__s1.y0)
                                view_bounds.__s1.y0 = y;
                            if (y > view_bounds.__s1.y1)
                                view_bounds.__s1.y1 = y;
                            if (z < view_bounds.__s1.z0)
                                view_bounds.__s1.z0 = z;
                            if (z > view_bounds.__s1.z1)
                                view_bounds.__s1.z1 = z;
                        }
                        vertex->color = vertex_color;
                        vertex->texcoord.n[1] = corner_v;
                        vertex->texcoord.n[0] = corner_u;

                        ++corner_index;
                        corner = (int16_t)(corner + 1);
                        if (corner >= 4)
                            break;
                    }

                    data->centroid.n[0] = data->centroid.n[0] + transformed_origin.n[0];
                    data->centroid.n[2] = data->centroid.n[2] + transformed_origin.n[2];
                    data->centroid.n[1] = data->centroid.n[1] + transformed_origin.n[1];
                    ++sprite_group->sprite_count;
                    ++data->sprite_count;

                    if ((data->flags & (1u << _build_sprite_viewer_space_bit)) == 0)
                    {
                        float view_fraction = render_frustum_cube_view_fraction(&render.frustum, &view_bounds);
                        build_sprite_globals.screen_coverage = view_fraction
                                + build_sprite_globals.screen_coverage;
                        if (view_fraction > 0.5f)
                        {
                            /* compare uses the pre-increment count */
                            if (build_sprite_globals.big_sprite_count++ > 10)
                            {
                                --sprite_group->sprite_count;
                                --data->sprite_count;
                            }
                        }
                        if (debug_sprites)
                        {
                            /* recovered: (real_point3d *)(vertices + 24*n) -> &vertices[n].position (position is at off 0) */
                            dynamic_screen_vertex *vertices =
                                    (dynamic_screen_vertex *)sprite_group->vertices;
                            real_point3d corner0_world;
                            real_point3d corner1_world;
                            real_point3d corner2_world;
                            real_point3d corner3_world;
                            matrix4x3_transform_point(&render.frustum.view_to_world,
                                    &vertices[corner_index - 4].position,
                                    &corner0_world);
                            matrix4x3_transform_point(&render.frustum.view_to_world,
                                    &vertices[corner_index - 3].position,
                                    &corner1_world);
                            matrix4x3_transform_point(&render.frustum.view_to_world,
                                    &vertices[corner_index - 2].position,
                                    &corner2_world);
                            matrix4x3_transform_point(&render.frustum.view_to_world,
                                    &vertices[corner_index - 1].position,
                                    &corner3_world);
                            rasterizer_debug_line(&corner0_world, &corner1_world, global_real_argb_white);
                            rasterizer_debug_line(&corner0_world, &corner2_world, global_real_argb_white);
                            rasterizer_debug_line(&corner2_world, &corner3_world, global_real_argb_white);
                            rasterizer_debug_line(&corner3_world, &corner1_world, global_real_argb_white);
                        }
                    }
                }
            }
        }
    }
}
