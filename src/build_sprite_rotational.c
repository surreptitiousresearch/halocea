/* build_sprite_rotational @0x837ECBC8 — draws a "rotational" sprite: a billboard that fakes a rotating
 * 3D object (rocket exhaust, spinning debris, etc.) out of 2D sprite frames. `first_sequence_index` names
 * TWO adjacent bitmap_group sequences: sequence[first_sequence_index] holds "equatorial" (side-on) view
 * frames, sequence[first_sequence_index+1] holds "polar" (looking along the rotation axis) view frames.
 * The angle between the view direction and the transformed rotation axis picks a cross-fade weight between
 * the two; near the poles the polar sequence dominates (optionally auto-advancing its own frame from
 * `rotation` when `flags & 2`), near the equator the side sequence dominates using a continuous on-screen
 * rotation angle from atan2() instead of a discrete frame.
 *
 * DEVIATION: decompiler output was severely corrupted ("local variable allocation has failed" — __int128/
 * DWORD1/DWORD2 register-pun locals, a used-but-never-assigned `v33` feeding `atan2`, and a bogus
 * `(const real_argb_color *)1` color argument). Fully rebuilt from disasm_range(0x837ECBC8, 0x837ECE5C).
 * The `(const real_argb_color *)1` was a decompiler stack-slot misattribution: that `1` is actually
 * build_sprite's `flags` argument (a stack-passed 11th param, per the established build_sprite extern);
 * the real color argument lands in a DIFFERENT stack slot the decompiler didn't show. Confirmed by tracing
 * both slots' raw stores side by side. The `1`/`3` inner build_sprite flags value is reproduced verbatim —
 * its bit meaning is unconfirmed since build_sprite itself isn't decompiled yet. */

#include <stdint.h>
#include "headers/build_sprite_data.h"
#include "headers/build_sprite_rotational_flags.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_argb_color.h"
#include "headers/global_tag_instances.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"

#include <math.h>
#include "headers/blam_data_globals.h"

extern const float one_over_full_circle; /* 1.0f / (2.0f * PI) — confirmed via raw dword 0x3E22F983 */

extern void build_sprite_transform_origin_and_direction(const build_sprite_data *data, unsigned int flags, const real_point3d *untransformed_origin, const real_vector3d *untransformed_direction, real_point3d *transformed_origin, real_vector3d *transformed_direction);
extern float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b);
extern void build_sprite(build_sprite_data *data, int16_t mode, int16_t sequence_index, int16_t sprite_index, const real_point3d *untransformed_origin, const real_vector3d *untransformed_direction, float rotation, float scale, const real_argb_color *color, float fade, unsigned int flags);

void build_sprite_rotational(build_sprite_data *data, unsigned int flags, int16_t first_sequence_index, int16_t sprite_index, const real_point3d *untransformed_origin, const real_vector3d *untransformed_axis_of_rotation, float rotation, float scale, const real_argb_color *color, float fade)
{
    real_point3d transformed_origin;
    real_vector3d transformed_axis;

    if (!color)
        color = global_real_argb_white;

    build_sprite_transform_origin_and_direction(data, flags & (1u << _build_sprite_rotational_viewer_space_bit),
            untransformed_origin,
            untransformed_axis_of_rotation, &transformed_origin, &transformed_axis);

    float angle = angle_between_vectors3d((const real_vector3d *)&transformed_origin, &transformed_axis);
    float angle_from_equator = angle - 1.5707964f; /* signed distance from pi/2 (the equator) */
    float pole_weight = angle_from_equator * angle_from_equator * 0.4052847f;

    if (pole_weight < 0.0f)
        pole_weight = 0.0f;
    else if (pole_weight > 1.0f)
        pole_weight = 1.0f;

    if (pole_weight > 0.05f)
    {
        bitmap_group *group = TAG_GET(bitmap_group, data->bitmap_group_index);
        bitmap_group_sequence *sequences = (bitmap_group_sequence *)group->sequences.address;
        __int16 pole_sprite_count = sequences[first_sequence_index + 1].sprites.count;
        __int16 frame_index;
        float inner_rotation;
        unsigned int inner_flags = 1;

        if (flags & (1u << _build_sprite_rotational_sideways_rotation_animates_bit))
        {
            double raw_frame = fmod((double)(pole_sprite_count * rotation * one_over_full_circle + 0.5f),
                    (double)pole_sprite_count);
            frame_index = (__int16)(raw_frame + sprite_index);
            if (angle_from_equator < 0.0f)
                frame_index = (__int16)(pole_sprite_count - sprite_index);
            inner_rotation = 0.0f;
        }
        else
        {
            frame_index = sprite_index;
            inner_rotation = rotation;
            if (angle_from_equator < 0.0f)
                inner_flags = 3;
        }

        build_sprite(data, 0, first_sequence_index + 1, frame_index, &transformed_origin, NULL, inner_rotation,
                scale, color, pole_weight * fade, inner_flags);
    }

    float equator_weight = 1.0f - pole_weight;
    if (equator_weight > 0.05f)
    {
        bitmap_group *group = TAG_GET(bitmap_group, data->bitmap_group_index);
        bitmap_group_sequence *sequences = (bitmap_group_sequence *)group->sequences.address;
        __int16 side_sprite_count = sequences[first_sequence_index].sprites.count;

        double raw_frame = fmod((double)(side_sprite_count * rotation * one_over_full_circle + 0.5f),
                (double)side_sprite_count);
        __int16 frame_index = (__int16)raw_frame;
        float screen_rotation = (float)atan2(transformed_axis.n[1], transformed_axis.n[0]);

        unsigned int equator_flags = 1;  /* same flag word as inner_flags base value in the pole arm */
        build_sprite(data, 0, first_sequence_index, frame_index, &transformed_origin, NULL, screen_rotation,
                scale, color, equator_weight * fade, equator_flags);
    }
}
