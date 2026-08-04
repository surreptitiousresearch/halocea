/* object_render_state_refresh @0x836E6A68 — bring an object's cached render lighting up to date.
 *
 * Decides whether the object's static lighting needs recomputing this frame: dynamically-lit objects refresh
 * on a cadence that scales with on-screen size (every frame above 400px, every 3 frames above 100px, every 10
 * frames otherwise). If the cached entry is stale (more than one frame old) a refresh is promoted to a full
 * rebuild. A static refresh recomputes the desired lighting and notes the frame; a scene change additionally
 * recomputes the dynamic (point-light) contribution. The displayed lighting is then reconciled with the desired
 * lighting: a full rebuild copies it wholesale; otherwise, if interpolation is enabled and the object is moving
 * (or is a biped), each lighting term eases toward its target, and the point-light selection is copied across.
 * Finally the scene/frame stamps are updated.
 *
 * DEVIATION: the decompiler emitted the cadence test as branchless sign-bit arithmetic
 * ((-x & ~x) >> 31 and an xor-sign/compare pair); both reduce to the frame-age comparisons written below.
 * The 4th parameter (r5) is never referenced by the function body — kept only to preserve the register ABI so
 * the 5th argument (the full-rebuild flag) lands in r6 as the callers pass it. */

#include <string.h>
#include <stdint.h>
#include "headers/data_array.h"
#include "headers/render_globals.h"
#include "headers/object_render_state.h"
#include "headers/real_vector3d.h"
#include "headers/object_header_datum.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

#define CACHED_RENDER_STATE_SIZE 256


#include "headers/real_rgb_color.h"
#include "headers/real_argb_color.h"
extern void lights_prepare_for_object_static(int object_index, render_lighting *lighting);
extern void lights_prepare_for_object_dynamic(int object_index, render_lighting *lighting);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void interpolate_real_rgb_color(real_rgb_color *current, const real_rgb_color *desired, float maximum_speed);
extern void interpolate_real_argb_color(real_argb_color *current, const real_argb_color *desired, float maximum_speed);
extern void interpolate_normal(real_vector3d *current, const real_vector3d *desired, float maximum_speed);

void object_render_state_refresh(int cache_index, int object_index, float level_of_detail_pixels,
                                 uint8_t unused_dirty, uint8_t force_full_rebuild)
{
    (void)unused_dirty;

    object_render_state *state = DATA_ARRAY_ELEMENT(cached_object_render_states, object_render_state, cache_index);

    int scene_age = render.scene_index - state->render_scene_index;
    int frame_age = render.frame_index - state->render_frame_index;
    if ( frame_age < 0 || scene_age < 0 )
    {
        frame_age = 1;
        scene_age = 1;
    }

    object_datum *object_data =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int is_dynamically_lit = object_data->object.flags & (1u << _object_static_lighting_recompute_bit);

    unsigned char needs_static_refresh = 0;
    if ( is_dynamically_lit )
    {
        if ( level_of_detail_pixels <= 400.0f )
        {
            int interval = (level_of_detail_pixels <= 100.0f) ? 10 : 3;
            needs_static_refresh = (frame_age > interval);
        }
        else
        {
            needs_static_refresh = (frame_age > 0);
        }
    }

    if ( needs_static_refresh && (render.frame_index - state->refresh_frame_index) > 1 && is_dynamically_lit )
        force_full_rebuild = 1;

    if ( force_full_rebuild || needs_static_refresh )
    {
        state->object_index = object_index;
        lights_prepare_for_object_static(object_index, &state->desired_lighting);
        state->lod_pixels = level_of_detail_pixels;
        state->refresh_frame_index = render.frame_index;
    }

    if ( force_full_rebuild || scene_age > 0 )
        lights_prepare_for_object_dynamic(object_index, &state->desired_lighting);

    if ( force_full_rebuild )
    {
        memcpy(&state->lighting, &state->desired_lighting, sizeof(state->lighting));
    }
    else if ( !needs_static_refresh )
    {
        if ( scene_age > 0 )
        {
            state->lighting.point_light_count = state->desired_lighting.point_light_count;
            state->lighting.point_light_indices[0] = state->desired_lighting.point_light_indices[0];
            state->lighting.point_light_indices[1] = state->desired_lighting.point_light_indices[1];
        }
    }
    else if ( !object_light_interpolate )
    {
        memcpy(&state->lighting, &state->desired_lighting, sizeof(state->lighting));
    }
    else
    {
        real_vector3d velocity;
        object_get_velocities(object_index, &velocity, 0);
        if ( velocity.n[0] != 0.0f || velocity.n[1] != 0.0f || velocity.n[2] != 0.0f
          || object_try_and_get_and_verify_type(object_index, object_mask_machine) )
        {
            interpolate_real_rgb_color(&state->lighting.ambient_color, &state->desired_lighting.ambient_color, 0.03f);
            interpolate_real_argb_color(&state->lighting.reflection_tint_color,
                                        &state->desired_lighting.reflection_tint_color, 0.03f);
            interpolate_real_rgb_color(&state->lighting.distant_lights[0].color,
                                       &state->desired_lighting.distant_lights[0].color, 0.03f);
            interpolate_normal(&state->lighting.distant_lights[0].direction,
                               &state->desired_lighting.distant_lights[0].direction, 0.03f);
            interpolate_real_rgb_color(&state->lighting.distant_lights[1].color,
                                       &state->desired_lighting.distant_lights[1].color, 0.03f);
            interpolate_normal(&state->lighting.distant_lights[1].direction,
                               &state->desired_lighting.distant_lights[1].direction, 0.03f);
            interpolate_normal(&state->lighting.shadow_vector, &state->desired_lighting.shadow_vector, 0.012f);
            interpolate_real_rgb_color(&state->lighting.shadow_color, &state->desired_lighting.shadow_color, 0.03f);
        }
        state->lighting.point_light_count = state->desired_lighting.point_light_count;
        state->lighting.point_light_indices[0] = state->desired_lighting.point_light_indices[0];
        state->lighting.point_light_indices[1] = state->desired_lighting.point_light_indices[1];
    }

    state->render_scene_index = render.scene_index;
    state->render_frame_index = render.frame_index;
}
