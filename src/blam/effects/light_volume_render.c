/* light_volume_render @0x8380AD30 — render one light-volume widget (a light shaft / god-ray) as a stack of
 * 3D sprites along the volume's attachment-marker axis. Resolves the volume definition and its interpolated
 * animation frame, finds the marker on the object, then computes an overall intensity from:
 *   - the view/marker parallel factor (|forward . marker_x_axis|) blended between the definition's
 *     perpendicular and parallel brightness scales,
 *   - an optional near/far distance fade,
 *   - an optional object function value.
 * If visible it emits `count` sprites; each sprite's position/radius/color/brightness is driven by a linear
 * fraction i/(count-1) shaped by per-attribute exponents (offset/radius/color/brightness), where the offset-
 * shaped fraction is the base for the other three.
 *
 * The decompiler emitted "local variable allocation has failed"; this reconstruction is disasm-verified
 * (0x8380AD30-0x8380B138).
 *
 * DEVIATIONS (all disasm-resolved):
 *  - The pow() calls are rendered as 128-bit long-double register puns: each is pow(base, exponent) guarded
 *    by `exponent != 1.0` (skip pow, keep base). The first pow's result (offset-shaped fraction) is the base
 *    for the radius/color/brightness pows.
 *  - rasterizer_widget_draw_sprite3d's color: the shipped call additionally packs green/blue into extra
 *    argument registers (r6/r7) that the database's 5-argument prototype for the widget does not expose, so
 *    (consistent with the decompiler and the rest of the corpus) only the (brightness<<8 | red) value the
 *    prototype's `scale` slot receives is passed; `color` is 0. The byte conversions are truncating (fctidz).
 *  - Marker fields use object_marker.matrix (object-space): row 0 is the volume axis, row 3 its origin. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/light_volume_globals.h"
#include "headers/light_volume_definition.h"
#include "headers/light_volume_flags.h"
#include "headers/light_volume_frame.h"
#include "headers/render_widget_type.h"
#include "headers/object_marker.h"
#include "headers/render_globals.h"
#include "headers/real_rgb_color.h"
#include "headers/real_point3d.h"
#include "headers/real_vector2d.h"
#include "headers/blam_data_globals.h"


extern void *datum_get(data_array *data, int index);
extern light_volume_frame * light_volume_interpolate_frames(const light_volume_definition *definition, int object_index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);
extern void rasterizer_widget_begin(int16_t type, uint16_t flags);
extern uint8_t rasterizer_widget_set_texture(int16_t stage_index, int bitmap_group_index, int16_t sequence_index);
extern void rasterizer_widget_draw_sprite3d(const real_point3d *point, float radius, const real_vector2d *scale, float rotation, unsigned int color);
extern void rasterizer_widget_end(void);
extern double pow(double base, double exponent);

/* shape a 0..1 fraction by a per-attribute exponent; the shipped code skips pow when the exponent is 1.0 */
static float light_volume_shape(float base, float exponent)
{
    if ( exponent != 1.0f )
        return (float)pow(base, exponent);
    return base;
}

void light_volume_render(int object_index, int widget_index)
{
    if ( object_index == -1 || widget_index == -1 )
        return;

    int definition_index = ((int *)datum_get(light_volume_globals.light_volume_data, widget_index))[1];
    light_volume_definition *definition = TAG_GET(light_volume_definition, definition_index);
    if ( definition->count <= 0 || definition->frames.count <= 0 )
        return;

    light_volume_frame *frame = light_volume_interpolate_frames(definition, object_index);

    object_marker marker;
    object_get_marker_by_name(object_index, definition->attachment_marker, &marker, 1);

    float parallel_factor = ((render.camera.forward.n[0] * marker.matrix.n[0][0])
            + ((marker.matrix.n[0][1] * render.camera.forward.n[1])
                    + (marker.matrix.n[0][2] * render.camera.forward.n[2])));
    if ( parallel_factor < 0.0f )
        parallel_factor = -parallel_factor;

    float distance_fade = 1.0f;
    if ( definition->far_fade_distance > 0.0f )
    {
        float depth = ((render.camera.forward.n[0]
                        * (marker.matrix.n[3][0] - render.camera.position.n[0]))
                + (((marker.matrix.n[3][2] - render.camera.position.n[2])
                                * render.camera.forward.n[2])
                        + ((marker.matrix.n[3][1] - render.camera.position.n[1])
                                * render.camera.forward.n[1])));
        float t = ((depth - definition->far_fade_distance)
                / (definition->near_fade_distance - definition->far_fade_distance));
        if ( t >= 0.0f )
            distance_fade = t <= 1.0f ? t : 1.0f;
        else
            distance_fade = 0.0f;
    }

    float angle_brightness = ((definition->perpendicular_brightness_scale
                    * (1.0f - parallel_factor))
            + (definition->parallel_brightness_scale * parallel_factor));
    float clamped_brightness;
    if ( angle_brightness >= 0.0f )
        clamped_brightness = angle_brightness <= 1.0f ? angle_brightness : 1.0f;
    else
        clamped_brightness = 0.0f;

    float intensity = (clamped_brightness * distance_fade);
    float function_value;
    if ( object_get_function_value(object_index, definition->brightness_scale_source - 1, &function_value) )
        intensity = (function_value * intensity);

    if ( intensity > 0.0f
      && (frame->color_hither.n[0] > 0.0f || frame->color_yon.n[0] > 0.0f)
      && (frame->radius_hither > 0.0f || frame->radius_yon > 0.0f) )
    {
        rasterizer_widget_begin(_widget_type_internal_sprite, 1);
        if ( !rasterizer_widget_set_texture(0, definition->map.index, definition->sequence_index) )
        {
            int count = definition->count;
            if ( count > 0 )
            {
                float divisor = (float)(count - 1);
                for ( int i = 0; i < count; i = (int16_t)(i + 1) )
                {
                    float base = ((float)i / divisor);
                    float offset_shaped = light_volume_shape(base, frame->offset_exponent);
                    float radius_shaped = light_volume_shape(offset_shaped, frame->radius_exponent);
                    float radius = (((1.0f - radius_shaped) * frame->radius_hither)
                            + (frame->radius_yon * radius_shaped));
                    float color_shaped = light_volume_shape(offset_shaped, frame->color_exponent);
                    float brightness_shaped = light_volume_shape(offset_shaped, frame->brightness_exponent);

                    float offset = ((frame->length * offset_shaped) + frame->offset_from_marker);
                    real_point3d sprite_position;
                    sprite_position.n[0] = ((offset * marker.matrix.n[0][0]) + marker.matrix.n[3][0]);
                    sprite_position.n[1] = ((marker.matrix.n[0][1] * offset) + marker.matrix.n[3][1]);
                    sprite_position.n[2] = ((marker.matrix.n[0][2] * offset) + marker.matrix.n[3][2]);

                    real_rgb_color sprite_color;
                    /* the two low flag bits are the color-interpolation controls passed through verbatim */
                    rgb_colors_interpolate(&sprite_color,
                            definition->flags & ((1u << _light_volume_color_interpolate_in_hsv_bit)
                                    | (1u << _light_volume_color_interpolate_along_farthest_hue_path_bit)),
                            &frame->color_hither.rgb,
                            &frame->color_yon.rgb, color_shaped);

                    float brightness = (((1.0f - brightness_shaped) * frame->color_hither.n[0])
                            + (frame->color_yon.n[0] * brightness_shaped)) * intensity;

                    /* (brightness << 8) | red; green/blue are dropped with the widget's extra color args */
                    unsigned int packed_color =
                            ((unsigned int)(uint8_t)(int64_t)(brightness * 255.0f) << 8)
                            | (uint8_t)(int64_t)(sprite_color.n[0] * 255.0f);
                    rasterizer_widget_draw_sprite3d(&sprite_position, radius,
                            (const real_vector2d *)packed_color, 0.0f, 0);
                }
            }
        }
        rasterizer_widget_end();
    }
}
