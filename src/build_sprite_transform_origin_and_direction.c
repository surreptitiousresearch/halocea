/* build_sprite_transform_origin_and_direction @0x837EBDA0 — transforms a sprite's untransformed
 * origin/direction into whatever space the caller's sprite batch expects. The decompiler fabricated
 * a spray of phantom trailing int params (a7..a20/a20) from register-allocator noise; disasm confirms
 * the real signature is the established 6-arg extern (data, flags, untransformed_origin,
 * untransformed_direction, transformed_origin, transformed_direction) used by build_sprite_rotational.
 *
 * DEVIATION / faithful quirk (confirmed via disasm_range(0x837EBDA0, 0x837EBE78)): when
 * `data->flags & 1` is set, this function calls render_camera_screen_to_view() with a throwaway stack
 * scratch buffer as its output and then returns immediately — `transformed_origin`/`transformed_direction`
 * are NEVER written in this branch. This looked like a bug on first read, but the compiled code genuinely
 * discards the computed view-space vector here; reproduced faithfully rather than "fixed". */

#include "headers/build_sprite_data.h"
#include "headers/build_sprites_flags.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/render_globals.h"

#include "headers/real_matrix4x3.h"
#include "headers/render_camera.h"
#include "headers/render_frustum.h"
extern void render_camera_screen_to_view(const render_camera *camera, const render_frustum *frustum, const real_point2d *screen_point, real_vector3d *view_vector);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);

void build_sprite_transform_origin_and_direction(const build_sprite_data *data, unsigned int flags,
        const real_point3d *untransformed_origin, const real_vector3d *untransformed_direction,
        real_point3d *transformed_origin, real_vector3d *transformed_direction)
{
    if (data->flags & (1u << _build_sprites_screen_space_bit))
    {
        /* result is discarded — see DEVIATION note above */
        real_vector3d discarded_view_vector;
        render_camera_screen_to_view(&render.camera, &render.frustum,
                (const real_point2d *)untransformed_origin, &discarded_view_vector);
        return;
    }

    if (flags & 1)
    {
        *transformed_origin = *untransformed_origin;
        if (untransformed_direction)
            *transformed_direction = *untransformed_direction;
        return;
    }

    matrix4x3_transform_point(&render.frustum.world_to_view, untransformed_origin, transformed_origin);
    if (untransformed_direction)
        matrix4x3_transform_normal(&render.frustum.world_to_view, untransformed_direction, transformed_direction);
}
