/* build_sprite_prepare_for_window @0x837EC158 — reset the per-window sprite-builder accumulators and cache
 * the world up/left axes transformed into viewer space (used to orient billboarded sprites toward the
 * camera).
 *
 * The final matrix4x3_transform_normal leaves its result pointer in r3 at the blr, but the sole caller
 * (render_window @0x83707348) ignores it — attested void.
 * Note: global_left3d is transformed into viewer_space_world_forward — the field name reflects how the
 * sprite builder consumes the left axis, not a relabelling of the input. */

#include "headers/build_sprite_globals.h"
#include "headers/render_globals.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

/* global_up3d / global_left3d are pointer globals: the call site loads a stored pointer (lwz) and passes it
 * directly as the real_vector3d *normal argument — no address-of. */
#include "headers/real_matrix4x3.h"
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);

void build_sprite_prepare_for_window(void)
{
    build_sprite_globals.screen_coverage = 0.0f;
    build_sprite_globals.big_sprite_count = 0;

    matrix4x3_transform_normal(&render.frustum.world_to_view, global_up3d,
                               &build_sprite_globals.viewer_space_world_up);
    matrix4x3_transform_normal(&render.frustum.world_to_view, global_left3d,
                               &build_sprite_globals.viewer_space_world_forward);
}
